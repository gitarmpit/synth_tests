#include "FFT.h"
#include <algorithm>

FFT::FFT()
{
  dBRange = 90;
  memset (&m_fftParam, 0, sizeof (m_fftParam));
  mProcessed = 0;
  mData = 0;
}

bool FFT::Run(int windowFunc, double sampleRate, size_t windowSize, size_t dataLen, float* data)
{
   mWindowFunc = windowFunc;
   mRate = sampleRate;
   mWindowSize = windowSize;
   mDataLen = dataLen;
   mData = (float*) realloc (mData, dataLen * sizeof(float));
   memcpy(mData, data, dataLen * sizeof(float));

   if (!(mWindowSize >= 32 && mWindowSize <= 65536 && mWindowFunc >= 0 && mWindowFunc <= 4)) 
   {
      fprintf (stderr, "check windowSize / windowFunc numbers\n");
      return false;
   }

   if (mDataLen < mWindowSize) {
      fprintf (stderr, "datalen < windowSize\n");
      return false;
   }

   size_t half = mWindowSize / 2;
   mProcessed = (float*) realloc (mProcessed, mWindowSize * sizeof(float));

   float* in  = (float*) malloc (mWindowSize * sizeof(float));
   float* out = (float*) malloc (mWindowSize * sizeof(float));
   float* win = (float*) malloc (mWindowSize * sizeof(float));
   
   for (size_t i = 0; i < mWindowSize; i++) {
      mProcessed[i] = 0.0f;
      win[i] = 1.0f;
   }

   WindowFunc(mWindowSize, win);

   // Scale window such that an amplitude of 1.0 in the time domain
   // shows an amplitude of 0dB in the frequency domain
   double wss = 0;
   for (size_t i = 0; i<mWindowSize; i++)
      wss += win[i];

   if(wss > 0)
      wss = 4.0 / (wss*wss);
   else
      wss = 1.0;

   size_t start = 0;
   int windows = 0;
   while (start + mWindowSize <= mDataLen) 
   {
      for (size_t i = 0; i < mWindowSize; i++)
         in[i] = win[i] * mData[start + i];
  
      PowerSpectrum(mWindowSize, in, out);

      for (size_t i = 0; i < half; i++)
         mProcessed[i] += out[i];

      start += half;
      windows++;
   }

   free (in);
   free (out);
   free (win);

   double scale;

   // Convert to decibels
   mYMin = 1000000.;
   mYMax = -1000000.;
   scale = wss / (double)windows;
   for (size_t i = 0; i < half; i++)
   {
     mProcessed[i] = 10 * log10(mProcessed[i] * scale);
     // printf ("proc: %d, %f\n", i, mProcessed[i]);
     if(mProcessed[i] > mYMax)
    mYMax = mProcessed[i];
     else if(mProcessed[i] < mYMin)
        mYMin = mProcessed[i];
   }

   if(mYMin < -dBRange)
     mYMin = -dBRange;

   if(mYMax <= -dBRange)
   {
      fprintf (stderr, "out of range\n");
      return false;
   }

   mYMax += .5;

   return true;
}
     
void FFT::CalculateView(int width, int height, bool logAxis) 
{
   //  Draw plot

   float yTotal = mYMax - mYMin;

   float yMax = mYMax;
   float yMin = yMax - yTotal;

   float xMin, xMax, xRatio, xStep;
 
   xMin = mRate / mWindowSize; // 44100 / 1024 = 43.066
   xMax = mRate / 2;  //22500
   xRatio = xMax / xMin; //512
   if (logAxis)
   {
      xStep = pow(2.0f, (log(xRatio) / log(2.0f)) / width);  // 1.004...
   }
   else
   {
      xStep = (xMax - xMin) / width;
   }
 
   float xPos = xMin;
   float y;
   float dbMax = -100.;
   float xposMax = 0;
 
   for (int i = 0; i < width; i++) 
   {
      if (logAxis)
         y = GetProcessedValue(xPos, xPos * xStep);
      else
         y = GetProcessedValue(xPos, xPos + xStep);

      float ynorm = (y - yMin) / yTotal;

      int lineheight = (int)(ynorm * (height - 1));

      //if (lineheight > r.height - 2)
      //   lineheight = r.height - 2;

      if (ynorm > 0.0)
      {
         fprintf(stderr, "i: %d, xPos: %7.3f, y: %7.3f, ynorm: %7.3f\n", i, xPos, y, ynorm);  
     fprintf(stderr, "ynorm > 0, line: x0=%d, y0=%d, x1=%d, y1=%d\n", 
              1 + i, height - 1 - lineheight,
              1 + i, height - 1);
          if (y > dbMax) 
          {
             dbMax = y;
             xposMax = xPos;
          }
      }

      if (logAxis)
         xPos *= xStep;
      else
         xPos += xStep;
   }

   printf ("dbMax: %f, freqMax: %f\n", dbMax, xposMax); 

}

float FFT::GetProcessedValue(float freq0, float freq1) const
{
   float bin0, bin1, binwidth;
   int half = mWindowSize / 2;

   bin0 = freq0 * mWindowSize / mRate;
   bin1 = freq1 * mWindowSize / mRate;
   binwidth = bin1 - bin0;

   float value = float(0.0);

   if (binwidth < 1.0) 
   {
      float binmid = (bin0 + bin1) / 2.0;
      int ibin = (int)(binmid) - 1;
      if (ibin < 1)
         ibin = 1;
      if (ibin >= half - 3)
         ibin = std::max(0, half - 4);

      value = CubicInterpolate(mProcessed[ibin],
                               mProcessed[ibin + 1],
                               mProcessed[ibin + 2],
                               mProcessed[ibin + 3], binmid - ibin);

   } 
   else 
   {
      if (bin0 < 0)
         bin0 = 0;

      if (bin1 >= half)
         bin1 = half - 1;

      if ((int)(bin1) > (int)(bin0))
         value += mProcessed[(int)(bin0)] * ((int)(bin0) + 1 - bin0);

      bin0 = 1 + (int)(bin0);
 
      while (bin0 < (int)(bin1)) 
      {
         value += mProcessed[(int)(bin0)];
         bin0 += 1.0;
      }

      value += mProcessed[(int)(bin1)] * (bin1 - (int)(bin1));
      value /= binwidth;
   }

   return value;
}

// If f(0)=y0, f(1)=y1, f(2)=y2, and f(3)=y3, this function finds
// the degree-three polynomial which best fits these points and
// returns the value of this polynomial at a value x.  Usually
// 0 < x < 3

float FFT::CubicInterpolate(float y0, float y1, float y2, float y3, float x) const
{
   float a, b, c, d;

   a = y0 / -6.0 + y1 / 2.0 - y2 / 2.0 + y3 / 6.0;
   b = y0 - 5.0 * y1 / 2.0 + 2.0 * y2 - y3 / 2.0;
   c = -11.0 * y0 / 6.0 + 3.0 * y1 - 3.0 * y2 / 2.0 + y3 / 3.0;
   d = y0;

   float xx = x * x;
   float xxx = xx * x;

   return (a * xxx + b * xx + c * x + d);
}


void FFT::InitFFT(size_t fftlen)
{
   m_fftParam.Points = fftlen / 2;
   if (m_fftParam.SinTable)
   {
      free (m_fftParam.SinTable);
   }

   m_fftParam.SinTable = (float*) malloc(fftlen * sizeof(float));

   if (m_fftParam.BitReversed)
   {
      free (m_fftParam.BitReversed);
   }

   m_fftParam.BitReversed = (int*) malloc(fftlen / 2 * sizeof(int));

   int temp;
   for(size_t i = 0; i < fftlen / 2; i++)
   {
      temp = 0;
      for(size_t mask = fftlen / 4; mask > 0; mask >>= 1)
         temp = (temp >> 1) + (i & mask ? m_fftParam.Points : 0);

      m_fftParam.BitReversed[i] = temp;
   }

   for(size_t i = 0; i < fftlen/2; i++)
   {
      m_fftParam.SinTable[m_fftParam.BitReversed[i]  ] = (float) -sin(2*M_PI*i/(fftlen));
      m_fftParam.SinTable[m_fftParam.BitReversed[i]+1] = (float) -cos(2*M_PI*i/(fftlen));
   }

}

void FFT::PowerSpectrum(size_t windowSize, const float *In, float *Out)
{
   InitFFT(windowSize);
   float* pFFT = (float*) malloc (windowSize * sizeof(float));

   for (size_t i = 0; i<windowSize; i++)
      pFFT[i] = In[i];


   RealFFT(pFFT);

   for (size_t i = 1; i<windowSize / 2; i++) {
      Out[i]= (pFFT[m_fftParam.BitReversed[i]  ]*pFFT[m_fftParam.BitReversed[i]  ])
         + (pFFT[m_fftParam.BitReversed[i]+1]*pFFT[m_fftParam.BitReversed[i]+1]);
   }

   // Handle the (real-only) DC and Fs/2 bins
   Out[0] = pFFT[0]*pFFT[0];
   Out[windowSize / 2] = pFFT[1]*pFFT[1];

   free (pFFT);
}

/*
*  Forward FFT routine.  Must call GetFFT(fftlen) first!
*
*  Note: Output is BIT-REVERSED! so you must use the BitReversed to
*        get legible output, (i.e. Real_i = buffer[ h->BitReversed[i] ]
*                                  Imag_i = buffer[ h->BitReversed[i]+1 ] )
*        Input is in normal order.
*
* Output buffer[0] is the DC bin, and output buffer[1] is the Fs/2 bin
* - this can be done because both values will always be real only
* - this allows us to not have to allocate an extra complex value for the Fs/2 bin
*
*  Note: The scaling on this is done according to the standard FFT definition,
*        so a unit amplitude DC signal will output an amplitude of (N)
*        (Older revisions would progressively scale the input, so the output
*        values would be similar in amplitude to the input values, which is
*        good when using fixed point arithmetic)
*/
void FFT::RealFFT(float *buffer)
{
   float *A,*B;
   const float *sptr;
   const float *endptr1,*endptr2;
   const int *br1,*br2;
   float HRplus,HRminus,HIplus,HIminus;
   float v1,v2,sin,cos;

   size_t ButterfliesPerGroup = m_fftParam.Points/2;

   /*
   *  Butterfly:
   *     Ain-----Aout
   *         \ /
   *         / \
   *     Bin-----Bout
   */

   endptr1 = buffer + m_fftParam.Points * 2;

   while(ButterfliesPerGroup > 0)
   {
      A = buffer;
      B = buffer + ButterfliesPerGroup * 2;
      sptr = m_fftParam.SinTable;

      while(A < endptr1)
      {
         sin = *sptr;
         cos = *(sptr+1);
         endptr2 = B;
         while(A < endptr2)
         {
            v1 = *B * cos + *(B + 1) * sin;
            v2 = *B * sin - *(B + 1) * cos;
            *B = (*A + v1);
            *(A++) = *(B++) - 2 * v1;
            *B = (*A - v2);
            *(A++) = *(B++) + 2 * v2;
         }
         A = B;
         B += ButterfliesPerGroup * 2;
         sptr += 2;
      }
      ButterfliesPerGroup >>= 1;
   }
   /* Massage output to get the output for a real input sequence. */
   br1 = m_fftParam.BitReversed + 1;
   br2 = m_fftParam.BitReversed + m_fftParam.Points - 1;

   while(br1<br2)
   {
      sin = m_fftParam.SinTable[*br1];
      cos = m_fftParam.SinTable[*br1+1];
      A=buffer+*br1;
      B=buffer+*br2;
      HRplus = (HRminus = *A     - *B    ) + (*B     * 2);
      HIplus = (HIminus = *(A+1) - *(B+1)) + (*(B+1) * 2);
      v1 = (sin*HRminus - cos*HIplus);
      v2 = (cos*HRminus + sin*HIplus);
      *A = (HRplus  + v1) * (float)0.5;
      *B = *A - v1;
      *(A+1) = (HIminus + v2) * (float)0.5;
      *(B+1) = *(A+1) - HIminus;

      br1++;
      br2--;
   }
   /* Handle the center bin (just need a conjugate) */
   A = buffer+*br1+1;
   *A = -*A;
   /* Handle DC bin separately - and ignore the Fs/2 bin
   buffer[0]+=buffer[1];
   buffer[1]=(float)0;*/
   /* Handle DC and Fs/2 bins separately */
   /* Put the Fs/2 value into the imaginary part of the DC bin */
   v1 = buffer[0] - buffer[1];
   buffer[0] += buffer[1];
   buffer[1] = v1;
}

void FFT::WindowFunc(int NumSamples, float* in)
{
    --NumSamples;
    if (mWindowFunc == eWinFuncHamming)
    {
        const double multiplier = 2 * M_PI / NumSamples;
        static const double coeff0 = 0.54, coeff1 = -0.46;
        for (int ii = 0; ii < NumSamples; ++ii)
            in[ii] *= coeff0 + coeff1 * cos(ii * multiplier);

        in[NumSamples] *= 0.8;

    }
    else if (mWindowFunc == eWinFuncHann)
    {
        const double multiplier = 2 * M_PI / NumSamples;
        static const double coeff0 = 0.5, coeff1 = -0.5;
        for (int ii = 0; ii < NumSamples; ++ii)
            in[ii] *= coeff0 + coeff1 * cos(ii * multiplier);

        in[NumSamples] *= 0.0;

    }
    else if (mWindowFunc == eWinFuncBlackman)
    {
        const double multiplier = 2 * M_PI / NumSamples;
        const double multiplier2 = 2 * multiplier;
        static const double coeff0 = 0.42, coeff1 = -0.5, coeff2 = 0.08;
        for (int ii = 0; ii < NumSamples; ++ii)
            in[ii] *= coeff0 + coeff1 * cos(ii * multiplier) + coeff2 * cos(ii * multiplier2);

        in[NumSamples] *= 0.0;
    }
    else if (mWindowFunc == eWinFuncBlackmanHarris) 
    {
        const double multiplier = 2 * M_PI / NumSamples;
        const double multiplier2 = 2 * multiplier;
        const double multiplier3 = 3 * multiplier;
        static const double coeff0 = 0.35875, coeff1 = -0.48829, coeff2 = 0.14128, coeff3 = -0.01168;
        for (int ii = 0; ii < NumSamples; ++ii)
            in[ii] *= coeff0 + coeff1 * cos(ii * multiplier) + coeff2 * cos(ii * multiplier2) + coeff3 * cos(ii * multiplier3);

        in[NumSamples] *= 0.0;
    }
    else
    {
        fprintf(stderr, "unexpected function number: %d", mWindowFunc);
        exit(1);
    }

}

FFT::~FFT()
{
  free (mProcessed);
  free (mData);
}

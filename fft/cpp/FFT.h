#ifndef __FFT_H
#define __FFT_H

#include <math.h>
#include <cstring>
#include <stdio.h>

struct FFTParam {
   int* BitReversed;
   float* SinTable;
   size_t Points;
};



enum eWindowFunctions
{
   eWinFuncHamming,
   eWinFuncHann,
   eWinFuncBlackman,
   eWinFuncBlackmanHarris
};



class FFT
{
public:
   FFT();
   ~FFT();

   bool Run(int windowFunc, double sampleRate, size_t windowSize, size_t dataLen, float* data);
   void CalculateView(int width, int height, bool logAxis);

 private:

   float GetProcessedValue(float freq0, float freq1) const;
   float CubicInterpolate(float y0, float y1, float y2, float y3, float x) const;

   void InitFFT(size_t fftlen);
   void PowerSpectrum(size_t NumSamples, const float *In, float *Out);
   void WindowFunc(int NumSamples, float *data);
   
   void RealFFT(float *buffer);

   int    mWindowFunc;
   int    dBRange;
   double mRate;
   size_t mDataLen;
   float* mData;
   size_t mWindowSize;
   float  mYMin;
   float  mYMax;
   
   FFTParam m_fftParam;

   float* mProcessed;

};

#endif

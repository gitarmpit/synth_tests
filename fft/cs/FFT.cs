using System;
using System.Collections.Generic;
using System.Text;

namespace FftSharp
{
    public enum WinFunc
    {
        eWinFuncHamming,
        eWinFuncHann,
        eWinFuncBlackman,
        eWinFuncBlackmanHarris
    }
    struct FFTParam
    {
        public int[] BitReversed;
        public float[] SinTable;
        public int Points;
    };

    public class FFT
    {
        private WinFunc mWindowFunc;
        private FFTParam m_fftParam;
        private int dBRange;
        private float mRate;
        private long mDataLen;
        private float[] mData;
        private int mWindowSize;
        private float mYMin;
        private float mYMax;

        //private List<float> mProcessed;
        private float[] mProcessed;

        public FFT()
        {
            dBRange = 90;
        }

        int GetProcessedSize()
        {
            return mProcessed.Length / 2;
        }

        public void Dump(float minDb)
        {
            float maxDb = -1000.0f;
            float maxFreq = 0;
            for (int i = 0; i < mWindowSize / 2; ++i)
            {
                float freq = i * mRate / mWindowSize;
                if (mProcessed[i] > minDb)
                {
                    Console.WriteLine(i + ": " +
                                      String.Format("{0:0.######}", mProcessed[i]) + ", freq: " +
                                      String.Format("{0:0.######}", freq));
                }
                if (mProcessed[i] > maxDb)
                {
                    maxDb = mProcessed[i];
                    maxFreq = freq;
                }

            }
            Console.WriteLine($"freq: {maxFreq}, db: {maxDb}");
        }

        public bool Run(WinFunc windowFunc, float sampleRate, int windowSize, long dataLen, float[] data)
        {
            mWindowFunc = windowFunc;
            mRate = sampleRate;
            mWindowSize = windowSize;
            mDataLen = dataLen;
            mData = data;  //TODO do we need to copy? 

            if (!(mWindowSize >= 32 && mWindowSize <= 65536))
            {
                Console.WriteLine("check windowSize");
                return false;
            }

            if (mDataLen < mWindowSize)
            {
                Console.WriteLine("datalen < windowSize\n");
                return false;
            }

            int half = mWindowSize / 2;
            mProcessed = new float[mWindowSize];

            float[] _in = new float[mWindowSize];
            float[] _out = new float[mWindowSize];
            double[] win = new double[mWindowSize];

            for (int i = 0; i < mWindowSize; i++)
            {
                mProcessed[i] = 0.0f;
                win[i] = 1.0f;
            }

            WindowFunc(mWindowSize, win);

            // Scale window such that an amplitude of 1.0 in the time domain
            // shows an amplitude of 0dB in the frequency domain
            double wss = 0;
            for (int i = 0; i < mWindowSize; i++)
                wss += win[i];

            if (wss > 0)
                wss = 4.0 / (wss * wss);
            else
                wss = 1.0;

            int start = 0;
            int windows = 0;
            while (start + mWindowSize <= mDataLen)
            {
                for (int i = 0; i < mWindowSize; i++)
                    _in[i] = (float)win[i] * mData[start + i];

                PowerSpectrum(mWindowSize, _in, _out);

                for (int i = 0; i < half; i++)
                    mProcessed[i] += _out[i];

                start += half;
                windows++;
            }


            double scale;

            // Convert to decibels
            mYMin = 1000000.0f;
            mYMax = -1000000.0f;
            scale = wss / (double)windows;
            for (int i = 0; i < half; i++)
            {
                mProcessed[i] = 10.0f * (float)Math.Log10(mProcessed[i] * scale);
                if (mProcessed[i] > mYMax)
                    mYMax = mProcessed[i];
                else if (mProcessed[i] < mYMin)
                    mYMin = mProcessed[i];
            }

            if (mYMin < -dBRange)
                mYMin = -dBRange;

            if (mYMax <= -dBRange)
            {
                Console.WriteLine("out of range\n");
                return false;
            }

            mYMax += .5f;

            return true;
        }

        public void CalculateView(int width, int height, bool logAxis)
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
                xStep = (float)(Math.Pow(2.0f, (Math.Log(xRatio) / Math.Log(2.0f)) / width));  // 1.004...
            }
            else
            {
                xStep = (xMax - xMin) / width;
            }

            float xPos = xMin;
            float y;
            float dbMax = -100.0f;
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
                //Console.WriteLine(String.Format("{0,4}", i) + ": " + mProcessed[i].ToString("F4"));
                if (ynorm > 0.0)
                {
                    Console.WriteLine($"i: {i}, xPos: {xPos}, y: {y}, ynorm: {ynorm}");
                    Console.WriteLine($"ynorm > 0, line: x0={1 + i}, y0={height - 1 - lineheight}, x1={1 + i}, y1={height - 1}");
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

            Console.WriteLine($"dbMax: {dbMax}, freqMax: {xposMax}");

        }



        float GetProcessedValue(float freq0, float freq1)
        {
            float bin0, bin1, binwidth;

            bin0 = freq0 * mWindowSize / mRate;
            bin1 = freq1 * mWindowSize / mRate;
            binwidth = bin1 - bin0;

            float value = 0.0f;

            if (binwidth < 1.0)
            {
                float binmid = (bin0 + bin1) / 2.0f;
                int ibin = (int)(binmid) - 1;
                if (ibin < 1)
                    ibin = 1;
                if (ibin >= GetProcessedSize() - 3)
                    ibin = Math.Max(0, GetProcessedSize() - 4);

                value = CubicInterpolate(mProcessed[ibin],
                                         mProcessed[ibin + 1],
                                         mProcessed[ibin + 2],
                                         mProcessed[ibin + 3], binmid - ibin);

            }
            else
            {
                if (bin0 < 0)
                    bin0 = 0;

                if (bin1 >= GetProcessedSize())
                    bin1 = GetProcessedSize() - 1;

                if ((int)(bin1) > (int)(bin0))
                    value += mProcessed[(int)(bin0)] * ((int)(bin0) + 1 - bin0);

                bin0 = 1 + (int)(bin0);

                while (bin0 < (int)(bin1))
                {
                    value += mProcessed[(int)(bin0)];
                    bin0 += 1.0f;
                }

                value += mProcessed[(int)(bin1)] * (bin1 - (int)(bin1));
                value /= binwidth;
            }

            return value;
        }

        void InitFFT(int fftlen)
        {
            m_fftParam.Points = fftlen / 2;
            m_fftParam.SinTable = new float[fftlen];
            m_fftParam.BitReversed = new int[fftlen / 2];

            int temp;
            for (int i = 0; i < fftlen / 2; i++)
            {
                temp = 0;
                for (int mask = fftlen / 4; mask > 0; mask >>= 1)
                    temp = (temp >> 1) + (((i & mask) > 0) ? m_fftParam.Points : 0);

                m_fftParam.BitReversed[i] = temp;
            }

            for (int i = 0; i < fftlen / 2; i++)
            {
                m_fftParam.SinTable[m_fftParam.BitReversed[i]] = (float)-Math.Sin(2 * Math.PI * i / (fftlen));
                m_fftParam.SinTable[m_fftParam.BitReversed[i] + 1] = (float)-Math.Cos(2 * Math.PI * i / (fftlen));
            }
        }

        void PowerSpectrum(int windowSize, float[] In, float[] Out)
        {
            InitFFT(windowSize);
            float[] pFFT = new float[windowSize];

            for (int i = 0; i < windowSize; i++)
                pFFT[i] = In[i];


            RealFFT(pFFT);

            for (int i = 1; i < windowSize / 2; i++)
            {
                Out[i] = (pFFT[m_fftParam.BitReversed[i]] * pFFT[m_fftParam.BitReversed[i]])
                   + (pFFT[m_fftParam.BitReversed[i] + 1] * pFFT[m_fftParam.BitReversed[i] + 1]);
            }

            // Handle the (real-only) DC and Fs/2 bins
            Out[0] = pFFT[0] * pFFT[0];
            Out[windowSize / 2] = pFFT[1] * pFFT[1];

        }

        void RealFFT(float[] buffer)
        {
            int br1, br2;
            float HRplus, HRminus, HIplus, HIminus;
            float v1, v2, sin, cos;

            int ButterfliesPerGroup = m_fftParam.Points / 2;
            int sptr;
            int A, B;

            int endptr1 = m_fftParam.Points * 2;
            int endptr2;

            while (ButterfliesPerGroup > 0)
            {
                A = 0;
                B = ButterfliesPerGroup * 2;
                sptr = 0;

                while (A < endptr1)
                {
                    sin = m_fftParam.SinTable[sptr];
                    cos = m_fftParam.SinTable[sptr + 1];
                    endptr2 = B;
                    while (A < endptr2)
                    {
                        v1 = buffer[B] * cos + buffer[B + 1] * sin;
                        v2 = buffer[B] * sin - buffer[B + 1] * cos;
                        buffer[B] = buffer[A] + v1;
                        buffer[A++] = buffer[B++] - 2 * v1;
                        buffer[B] = buffer[A] - v2;
                        buffer[A++] = buffer[B++] + 2 * v2;
                    }
                    A = B;
                    B += ButterfliesPerGroup * 2;
                    sptr += 2;
                }
                ButterfliesPerGroup >>= 1;
            }

            for (int i = 0; i < m_fftParam.Points; ++i)
            {
                // Console.WriteLine(i + ": " + String.Format("{0:0.######}", buffer[i]));
            }

            /* Massage output to get the output for a real input sequence. */
            br1 = 1;
            br2 = m_fftParam.Points - 1;

            while (br1 < br2)
            {
                sin = m_fftParam.SinTable[m_fftParam.BitReversed[br1]];
                cos = m_fftParam.SinTable[m_fftParam.BitReversed[br1] + 1];
                A = m_fftParam.BitReversed[br1];
                B = m_fftParam.BitReversed[br2];
                HRminus = buffer[A] - buffer[B];
                HRplus = HRminus + buffer[B] * 2;
                HIminus = buffer[A + 1] - buffer[B + 1];
                HIplus = HIminus + buffer[B + 1] * 2;
                v1 = sin * HRminus - cos * HIplus;
                v2 = cos * HRminus + sin * HIplus;
                buffer[A] = (HRplus + v1) * (float)0.5;
                buffer[B] = buffer[A] - v1;
                buffer[A + 1] = (HIminus + v2) * (float)0.5;
                buffer[B + 1] = buffer[A + 1] - HIminus;

                br1++;
                br2--;
            }

            /* Handle the center bin (just need a conjugate) */
            A = m_fftParam.BitReversed[br1] + 1;
            buffer[A] = -buffer[A];
            /* Handle DC bin separately - and ignore the Fs/2 bin
            buffer[0]+=buffer[1];
            buffer[1]=(float)0;*/
            /* Handle DC and Fs/2 bins separately */
            /* Put the Fs/2 value into the imaginary part of the DC bin */
            v1 = buffer[0] - buffer[1];
            buffer[0] += buffer[1];
            buffer[1] = v1;
        }

        void WindowFunc(int NumSamples, double[] _in)
        {
            --NumSamples;
            if (mWindowFunc == WinFunc.eWinFuncHamming)
            {
                double multiplier = 2 * Math.PI / NumSamples;
                const double coeff0 = 0.54, coeff1 = -0.46;
                for (int ii = 0; ii < NumSamples; ++ii)
                    _in[ii] *= coeff0 + coeff1 * Math.Cos(ii * multiplier);

                _in[NumSamples] *= 0.8;

            }
            else if (mWindowFunc == WinFunc.eWinFuncHann)
            {
                double multiplier = 2 * Math.PI / NumSamples;
                const double coeff0 = 0.5, coeff1 = -0.5;
                for (int ii = 0; ii < NumSamples; ++ii)
                    _in[ii] *= coeff0 + coeff1 * Math.Cos(ii * multiplier);

                _in[NumSamples] *= 0.0;

            }
            else if (mWindowFunc == WinFunc.eWinFuncBlackman)
            {
                double multiplier = 2 * Math.PI / NumSamples;
                double multiplier2 = 2 * multiplier;
                const double coeff0 = 0.42, coeff1 = -0.5, coeff2 = 0.08;
                for (int ii = 0; ii < NumSamples; ++ii)
                    _in[ii] *= coeff0 + coeff1 * Math.Cos(ii * multiplier) + coeff2 * Math.Cos(ii * multiplier2);

                _in[NumSamples] *= 0.0;
            }
            else if (mWindowFunc == WinFunc.eWinFuncBlackmanHarris)
            {
                double multiplier = 2 * Math.PI / NumSamples;
                double multiplier2 = 2 * multiplier;
                double multiplier3 = 3 * multiplier;
                const double coeff0 = 0.35875, coeff1 = -0.48829, coeff2 = 0.14128, coeff3 = -0.01168;
                for (int ii = 0; ii < NumSamples; ++ii)
                    _in[ii] *= coeff0 + coeff1 * Math.Cos(ii * multiplier) + coeff2 * Math.Cos(ii * multiplier2) + coeff3 * Math.Cos(ii * multiplier3);

                _in[NumSamples] *= 0.0;
            }
            else
            {
                throw new Exception($"unexpected window function: {mWindowFunc}");
            }

        }
        float CubicInterpolate(float y0, float y1, float y2, float y3, float x)
        {
            float a, b, c, d;

            a = y0 / -6.0f + y1 / 2.0f - y2 / 2.0f + y3 / 6.0f;
            b = y0 - 5.0f * y1 / 2.0f + 2.0f * y2 - y3 / 2.0f;
            c = -11.0f * y0 / 6.0f + 3.0f * y1 - 3.0f * y2 / 2.0f + y3 / 3.0f;
            d = y0;

            float xx = x * x;
            float xxx = xx * x;

            return (a * xxx + b * xx + c * x + d);
        }

    }


}

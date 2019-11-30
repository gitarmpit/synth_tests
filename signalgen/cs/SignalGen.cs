using System;
using System.Collections.Generic;
using System.Text;

namespace FftSharp
{
    class SignalGen
    {
        const int maxHarmonics = 10;
        const int table_size = 1024;
        float[] table = new float[table_size];
        float[] table_c = new float[table_size];
        float[] _out;
        public SignalGen ()
        {
            for (int i = 0; i < table_size; ++i)
            {
                table[i] = (float)Math.Sin(2.0f * Math.PI / (float)table_size * (float)i);
                table_c[i] = (float)Math.Cos(2.0f * Math.PI / (float)table_size * (float)i);
            }
        }

        public bool Generate(int freq, float[] harmAmps, int sps, float duration_sec)
        {
            int npoints = (int)(sps * duration_sec);
            _out = new float[npoints];
            for (int i = 0; i < npoints; ++i)
            {
                _out[i] = 0;
            }

            if (harmAmps.Length > maxHarmonics)
            {
                Console.WriteLine($"Too many harmonics. Max={maxHarmonics}");
            }

            
            for (int nharm = 0; nharm < harmAmps.Length; ++ nharm)
            {
                if (!GenerateFreq(freq * (nharm+1), harmAmps[nharm], sps, npoints))
                {
                    return false;
                }
            }

            return true;
        }

        public float[] Get()
        {
            return _out;
        }

        private float getPoint (int idx, bool isSin)
        {
            return isSin ? table[idx] : table_c[idx];
        }

        private bool GenerateFreq(int freq, float amp, int sps, int npoints)
        {
            float phase = 0.0f;
            bool sin = true;
            if (amp < 0)
            {
                sin = false;
                amp = -amp;
                phase = table_size / 4; // 90 degrees
            }

            Console.Error.WriteLine($"freq: {freq}, sin:{sin}, amp: {amp}");

            float phase_step = (float)freq / (float)sps * (float)table_size;
            if ((int)phase_step > table_size / 2)
            {
                Console.WriteLine($"error: phase_step too high: {phase_step}");
                return false;
            }

            float val;
            for (int i = 0; i < npoints; ++i)
            {
                int idx0 = (int)phase;

                float v0 = table[idx0]; //getPoint(idx0, sin); 
                //float v0 = getPoint(idx0, sin); 
                if (i < npoints - 1)
                {
                    int idx1 = idx0 + 1;
                    if (idx1 > table_size - 1)
                    {
                        idx1 = 0;
                    }
                    float v1 = table[idx1]; //getPoint(idx1, sin); 
                    // float v1 = getPoint(idx1, sin); 
                    float frac = phase - (float)idx0;
                    val = v0 + (v1 - v0) * frac;
                }
                else
                {
                    val = v0;
                }

                _out[i] += (val * (float)amp);
                phase += phase_step;
                if (phase >= (float)table_size)
                    phase -= (float)table_size;

                if ((int)phase > table_size - 1)
                {
                    Console.WriteLine("error: phase > table size");
                    Environment.Exit(1);
                }

            }
            return true;
        }
    }
}

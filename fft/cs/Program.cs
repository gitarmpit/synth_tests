using System;

namespace FftSharp
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 5)
            {
                Console.WriteLine("Args: <sps> <win> <freq> <sec> <minDb>");
                Environment.Exit(1);
            }

            int sps = 0;
            int window_size = 0;
            int freq = 0;
            int duration_sec = 0;
            float minDb = -60;

            try
            {
                sps = int.Parse(args[0]);
                window_size = int.Parse(args[1]);
                freq = int.Parse(args[2]);
                duration_sec = int.Parse(args[3]);
                minDb = float.Parse(args[4]);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
                Environment.Exit(1);
            }

            if (sps < 1000 || sps > 44100)
            {
                Console.WriteLine($"Sps: {sps}, has to be >= 1000 <= 44100");
                Environment.Exit(1);
            }

            if (freq < 0 || freq >= sps / 2)
            {
                Console.WriteLine($"Freq: {freq}, has to be > 0 <{sps/2}");
                Environment.Exit(1);
            }

            if (window_size < 128 || window_size > 4096)
            {
                Console.WriteLine($"win: {window_size}, has to be >= 128 <= 4096");
                Environment.Exit(1);
            }

            if (duration_sec < 1 && duration_sec > 60)
            {
                Console.WriteLine($"sec: {duration_sec}, has to be >0 <=60");
                Environment.Exit(1);
            }

            if (minDb > -10.0f)
            {
                Console.WriteLine($"minDb: {minDb}, has to be < -10");
                Environment.Exit(1);
            }

            int windowSize =
               // windowSize < 256 too inaccurate
               (int)(Math.Max(256, Math.Round(Math.Pow(2.0, Math.Floor((Math.Log(sps / 20.0) / Math.Log(2.0)) + 0.5)))));

            //Console.WriteLine(windowSize);

            //Environment.Exit(1);

            Console.WriteLine(windowSize);
            const int table_size = 1024;
            float[] table = new float[table_size];
            for (int i = 0; i < table_size; ++i)
            {
                table[i] = (float)Math.Sin(2.0f * Math.PI / (float)table_size * (float)i);
            }

            float phase = 0.0f;
            float phase_step = (float)freq / (float)sps * (float)table_size;

            if ((int)phase_step > table_size / 2)
            {
                Console.WriteLine($"error: phase_step too high: {phase_step}");
                // exit(1);
            }


            int npoints = sps * duration_sec;

            float[] wav = new float[npoints];

            Console.WriteLine($"sine freq={freq}");
            Console.WriteLine($"sample duration, sec: {duration_sec}");
            Console.WriteLine($"sps: {sps}");
            Console.WriteLine($"table_size={table_size}");
            Console.WriteLine($"lookup table phase_step: {phase_step}");
            Console.WriteLine($"total samples in wav: {npoints}");

            float val;
            for (int i = 0; i < npoints; ++i)
            {
                int idx0 = (int)phase;

                float v0 = table[idx0];
                if (i < npoints - 1)
                {
                    int idx1 = idx0 + 1;
                    if (idx1 > table_size - 1)
                    {
                        idx1 = 0;
                    }
                    float v1 = table[idx1];
                    float frac = phase - (float)idx0;
                    val = v0 + (v1 - v0) * frac;
                }
                else
                {
                    val = v0;
                }

                short w = (short)(32767f * val);
                // printf ("%7.4f\n", (double)w / (double) 32767);

                wav[i] = (float)w / 32767f;
                //Console.WriteLine($"{i}: {wav[i]}");
                phase += phase_step;
                if (phase >= (float)table_size)
                    phase -= (float)table_size;

                if ((int)phase > table_size - 1)
                {
                    Console.WriteLine("error: phase > table size");
                    Environment.Exit(1);
                }

            }


            bool logAxis = true;
            FFT fft = new FFT();
            //WinFunc f = WinFunc.eWinFuncBlackmanHarris;
            // eWinFuncBlackmanHarris is the narrowest
            // Then BlackMan then Hann them Hamming is really wide
            fft.Run(WinFunc.eWinFuncBlackmanHarris, (float) sps, window_size, npoints, wav);
            fft.Dump(minDb);
            // fft.CalculateView(500, 400, true);
        }
    }
}

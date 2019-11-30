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

            string[] sarry = args[0].Split(",");
            float[] harms = new float[sarry.Length];
            for (int i = 0; i < sarry.Length; ++i)
            {
                harms[i] = float.Parse(sarry[i]);
                Console.Error.WriteLine($"{i+1}: {harms[i]}");
            }

            float dur_sec = float.Parse(args[1]);

            SignalGen sgen = new SignalGen();
            if (sgen.Generate(10, harms, 1000, dur_sec))
            {
                float[] res = sgen.Get();
                for (int i = 0; i < res.Length; ++i)
                {
                    Console.WriteLine($"{i}, {res[i].ToString("F8")}");
                }
            }
        } 
    }
}

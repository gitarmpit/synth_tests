#include "SignalGen.h" 
#include <math.h>

static double curvee(double x) 
{
  return pow(fabs(fmod (fabs(x), M_PI*2) - M_PI), 2) / M_PI / M_PI * 2 - 1;
}

static double curvee2(double x) 
{
  return pow(fabs(fmod (fabs(x), M_PI*2) - M_PI), 0.5) / sqrt(M_PI) * 2 - 1;
}

static double triangle(double x)
{
   return fabs(fmod (fabs(x), M_PI*2) - M_PI) / M_PI * 2 - 1;
}

static double sawtooth (double x) 
{
   return -(fabs(fmod (fabs(x), M_PI*2)) / M_PI - 1);
}

static double square (double x) 
{
   return fabs (fmod (fabs(x), M_PI*2) < M_PI ? 1 : 0) - 0.5;
}

static double square2 (double x) 
{
   static const double scale = M_PI/10;
   static double thr1 = M_PI - scale;
   static double thr2 = 2*M_PI - scale;

   double phase = fmod (fabs(x), M_PI*2);

   if (phase >= thr1 && phase <= M_PI) 
   {
      return triangle (M_PI/scale*(phase-thr1));
   }
   else if (phase >=thr2) 
   {
     return triangle (M_PI + M_PI/scale*(phase-thr2));
   }
   else 
   {
     return fabs(   phase < M_PI ? 2 : 0  ) - 1;
   }
}

SignalGen::SignalGen(int sps)
{
    _out = 0;
    _npoints = 0;
    _sps = sps;
    for (int i = 0; i < table_size; ++i)
    {
        _table[i] = (double)cos(2.0f * M_PI / (double)table_size * (double)i);
    }
}

SignalGen::~SignalGen()
{
    delete[] _out;
}

bool SignalGen::init (double duration_sec)
{
    _npoints = (int)(_sps * duration_sec);
    if (_npoints == 0) 
    {
      fprintf (stderr, "nsamples=0\n");
      return false;
    }

    if (_out)
    {
        delete[] _out;
    }
    _out = new double[_npoints];
    for (int i = 0; i < _npoints; ++i)
    {
        _out[i] = 0;
    }
    return true;
}

bool SignalGen::GenerateOne(int freq, double amp, double& phase, double duration_sec, bool cont)
{
    if (!init(duration_sec)) 
    {
        return false;
    }
    return GenerateOne(freq, amp, phase, cont);
}

bool SignalGen::Generate(int freq, const std::vector<double>& harmAmps, const std::vector<double>& phases, double duration_sec, bool cont)
{
    if (!init(duration_sec)) 
    {
        return false;
    }

    if (harmAmps.size() > maxHarmonics)
    {
        fprintf(stderr, "Too many harmonics. Max=%d\n", maxHarmonics);
        return false;  
    }

    if (harmAmps.size() != phases.size()) 
    {
       fprintf (stderr, "harmonics and phase arrays must be the same size");
       return false;
    }

    for (size_t nharm = 0; nharm < harmAmps.size(); ++nharm)
    {
        int f = freq * (nharm + 1);
        //if (fabs(harmAmps[nharm]) < 0.00000001) 
        //{
        //    fprintf(stderr, "skipping 0 harmonic: %d n=%d\n", f, nharm+1);
        //}
        if (f >= _sps/2) 
        {
            fprintf(stderr, "sps too low for freq: %d\n", f);
        }
        else if (f > 20000)
        {
            fprintf(stderr, "freq too high: %d\n", f);
        }
        else
        {
            double phase = phases[nharm];
            if (!GenerateOne(f, harmAmps[nharm], phase, cont))
            {
                return false;
            }
        }
    }

    return true;
}

bool SignalGen::GenerateOne(int freq, double amp, double& phase, bool cont)
{
    if (phase < 0) 
    {
       phase = 2*M_PI + phase;
    }



    //fprintf (stderr, "freq: %d, amp: %f, phase (deg): %f\n", freq, amp, phase*180/M_PI);
    if (!cont) 
    {
        phase = table_size / (2*M_PI) * phase;
    }
    //fprintf (stderr, "idx: %f\n", phase);

    if (phase >= (double)table_size)
    {
       fprintf (stderr, "phase too high: %f\n", phase);
       return false;
    }
   
    double phase_step = (double)freq / (double)_sps * (double)table_size;
    if ((int)phase_step > table_size / 2)
    {
        fprintf(stderr, "error: phase_step too high: %f\n", phase_step);
        return false;
    }

    //fprintf (stderr, "idx: %f\n", phase);

    double val;
    double max_err = 0;
    for (int i = 0; i < _npoints; ++i)
    {
        int idx0 = (int)phase;

        double v0 = _table[idx0]; 
        int idx1 = idx0 + 1;
        if (idx1 > table_size - 1)
        {
           idx1 = 0;
        }
        double v1 = _table[idx1];  
        double frac = phase - (double)idx0;
        val = v0 + (v1 - v0) * frac;

        _out[i] += (val * (double)amp);
        double t = (double)i / (double)_sps;
        double v = amp * cos (2*M_PI*freq*t); 
        //printf ("%.18g\n", v);
        double err = fabs(v - _out[i]) / fabs(v);
        if (fabs(v) > 0.00000000001 && err > max_err) 
        {
          max_err = err;
          //printf ("max err: %f, t:%f\n", max_err*100, t); 
        }
        //printf ("t: %.18g, table: %.18g, actual: %.18g, phase: %f, err: %f, max err: %f\n", t, _out[i], v, phase, err*100, max_err*100);  
        phase += phase_step;
        if (phase >= (double)table_size)
        {
            phase -= (double)table_size;
        }

        if ((int)phase > table_size - 1)
        {
            fprintf(stderr, "error: phase > table size");
            exit(1);
        }

    }
    return true;
}


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

static float circle (float x) 
{
   float m = (float) fmod(x+M_PI/2, M_PI*2);
   if (m < 0) 
     m += (float)M_PI*2;

   if (m > M_PI*2) 
   {
     m -= (float)M_PI*2;
   }
    
   float theta;

   if (m <= M_PI)
   {
      theta = (float)acos ((m-M_PI/2)/M_PI*2);
   }
   else
   {
      theta = (float)-acos ((-M_PI+m-M_PI/2)/M_PI*2);
   }  
      
   return (float)sin(theta);

}


SignalGen::SignalGen(int sps, int shape)
{
    _out = 0;
    _nsamples = 0;
    _sps = sps;
    _maxAmp = 0;

    for (int i = 0; i < table_size; ++i)
    {
        if (shape == 1) 
        {
           _table[i] = (double)circle(2.0f * M_PI / (double)table_size * (double)i);
        }
        else if (shape == 2) 
        {
           _table[i] = (double)curvee(2.0f * M_PI / (double)table_size * (double)i);
        }
        else if (shape == 3) 
        {
           _table[i] = (double)curvee2(2.0f * M_PI / (double)table_size * (double)i);
        }
        else if (shape == 4) 
        {
           _table[i] = (double)square(2.0f * M_PI / (double)table_size * (double)i);
        }
        else if (shape == 5) 
        {
           _table[i] = (double)square2(2.0f * M_PI / (double)table_size * (double)i);
        }
        else if (shape == 6) 
        {
           _table[i] = (double)triangle(2.0f * M_PI / (double)table_size * (double)i);
        }
        else if (shape == 7) 
        {
           _table[i] = (double)sawtooth(2.0f * M_PI / (double)table_size * (double)i);
        }
        else 
        {
           _table[i] = (double)cos(2.0f * M_PI / (double)table_size * (double)i);
        }
    }
}

SignalGen::~SignalGen()
{
    delete[] _out;
}

bool SignalGen::init (double duration_sec)
{
    _nsamples = (int)(_sps * duration_sec);
    if (_nsamples == 0) 
    {
      fprintf (stderr, "nsamples=0\n");
      return false;
    }

    if (_out)
    {
        delete[] _out;
    }
    _out = new double[_nsamples];
    for (int i = 0; i < _nsamples; ++i)
    {
        _out[i] = 0;
    }
    return true;
}

bool SignalGen::GenerateOne(int freq, double amp, double& phase, double duration_sec)
{
    if (!init(duration_sec)) 
    {
       return false;
    }

    if (!GenerateOne(freq, amp, phase)) 
    {
       return false;
    }

    if (freq > 0 && amp > 0) 
    {
       // ApplyEnvelope (amp); 
    }

    return true;
}

bool SignalGen::Generate(int freq, const std::vector<double>& harmAmps, const std::vector<double>& phases, double duration_sec)
{
    if (!init(duration_sec)) 
    {
        return false;
    }

    if (harmAmps.size() > (size_t)maxHarmonics)
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
            if (!GenerateOne(f, harmAmps[nharm], phase))
            {
                return false;
            }
        }
    }

    if (freq > 0) 
    {
       //ApplyEnvelope (_maxAmp);
    }

    return true;
}

void SignalGen::ApplyEnvelope(double amp) 
{
    int A_ms = 50;
    int D_ms = 50;
    int R_ms = 50;
    double S_level = amp*0.7; 
    double A_len = _sps / 1000.0 * A_ms;
    double D_len = _sps / 1000.0 * D_ms;
    double R_len = _sps / 1000.0 * R_ms;
     
    double m_A = amp / A_len; 
    double m_D = - (amp - S_level) / D_len;
    double b_D = amp - m_D*A_len;
    double m_R = -S_level / R_len;
    double b_R = S_level - m_R*(_nsamples - R_len);
    
    double _amp = amp; 
    for (int i = 0; i < _nsamples; ++i)
    {
        if (i < A_len) 
        {
          _amp = i * m_A;
        }
        else if (i < A_len + D_len) 
        {
          _amp = i * m_D + b_D;
        }
        else if (i > _nsamples - R_len)
        {
          _amp = i * m_R + b_R;
        }
        _out[i] = _amp;
    }
}

bool SignalGen::GenerateOne(int freq, double amp, double& phase)
{
    if (phase < 0) 
    {
       phase = 2*M_PI + phase;
    }

    if (phase < 0 || phase >= 2 * M_PI)
    {
        fprintf(stderr, "invalid phase: %f", phase);
    }

    double table_idx = table_size / (2*M_PI) * phase;
    
    double phase_step = (double)freq / (double)_sps * (double)table_size;
    if ((int)phase_step > table_size / 2)
    {
        fprintf(stderr, "error: phase_step too high: %f\n", phase_step);
        return false;
    }

    for (int i = 0; i < _nsamples; ++i)
    {
        if (freq == 0 || amp == 0) 
        {
          _out[i] += 0;
          continue;
        }

        int idx0 = (int)table_idx;

        double v0 = _table[idx0]; 
        int idx1 = idx0 + 1;
        if (idx1 > table_size - 1)
        {
           idx1 = 0;
        }
        
        double v1 = _table[idx1];  
        double frac = table_idx - (double)idx0;
        double val = v0 + (v1 - v0) * frac;
 
        _out[i] += val * amp;

        if (_out[i] > _maxAmp) 
        {
            _maxAmp = _out[i];
        }

        table_idx += phase_step;
        if (table_idx >= (double)table_size)
        {
            table_idx -= (double)table_size;
        }

        if ((int)table_idx > table_size - 1)
        {
            fprintf(stderr, "error: table_idx > table size");
            exit(1);
        }

    }
    
    phase = table_idx  * 2 * M_PI / table_size;
    //fprintf (stderr, "end: table_idx: %f, phase: %f\n", table_idx, phase);
    return true;
}



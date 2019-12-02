#include "SignalGen.h" 
#include <math.h>

static float curvee(float x) 
{
  return pow(fabs(fmod (fabs(x), M_PI*2) - M_PI), 2) / M_PI / M_PI * 2 - 1;
}

static float curvee2(float x) 
{
  return pow(fabs(fmod (fabs(x), M_PI*2) - M_PI), 0.5) / sqrt(M_PI) * 2 - 1;
}

static float triangle(float x)
{
   return fabs(fmod (fabs(x), M_PI*2) - M_PI) / M_PI * 2 - 1;
}

static float sawtooth (float x) 
{
   return -(fabs(fmod (fabs(x), M_PI*2)) / M_PI - 1);
}

static float square (float x) 
{
   return fabs (fmod (fabs(x), M_PI*2) < M_PI ? 1 : 0) - 0.5;
}

static float square2 (float x) 
{
   static const float scale = M_PI/10;
   static float thr1 = M_PI - scale;
   static float thr2 = 2*M_PI - scale;

   float phase = fmod (fabs(x), M_PI*2);

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
        _table[i] = (float)cos(2.0f * M_PI / (float)table_size * (float)i);
    }
}

SignalGen::~SignalGen()
{
    delete[] _out;
}

void SignalGen::init (float duration_sec)
{
    _npoints = (int)(_sps * duration_sec);
    if (_out)
    {
        delete[] _out;
    }
    _out = new float[_npoints];
    for (int i = 0; i < _npoints; ++i)
    {
        _out[i] = 0;
    }
}

bool SignalGen::GenerateOne(int freq, float amp, float& phase, float duration_sec)
{
    init(duration_sec);
    return GenerateOne(freq, amp, phase);
}

bool SignalGen::Generate(int freq, const std::vector<float>& harmAmps, const std::vector<float>& phases, float duration_sec)
{
    init(duration_sec);
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
        if (fabs(harmAmps[nharm]) < 0.00000001) 
        {
            fprintf(stderr, "skipping 0 harmonic: %d n=%d\n", f, nharm+1);
        }
        else if (f >= _sps/2) 
        {
            fprintf(stderr, "sps too low for freq: %d\n", f);
        }
        else if (f > 20000)
        {
            fprintf(stderr, "freq too high: %d\n", f);
        }
        else
        {
            float phase = phases[nharm];
            if (!GenerateOne(f, harmAmps[nharm], phase))
            {
                return false;
            }
        }
    }

    return true;
}

bool SignalGen::GenerateOne(int freq, float amp, float& phase)
{
    if (amp < 0)
    {
        //amp = -amp;
        //phase = -M_PI/2; // -90 degrees
    }

    if (phase < 0) 
    {
       phase = 2*M_PI + phase;
    }

    //fprintf (stderr, "freq: %d, amp: %f, phase (deg): %f\n", freq, amp, phase*180/M_PI);
    phase = table_size / (2*M_PI) * phase;
    //fprintf (stderr, "idx: %f\n", phase);
    
    float phase_step = (float)freq / (float)_sps * (float)table_size;
    if ((int)phase_step > table_size / 2)
    {
        fprintf(stderr, "error: phase_step too high: %f\n", phase_step);
        return false;
    }

    float val;
    for (int i = 0; i < _npoints; ++i)
    {
        int idx0 = (int)phase;

        float v0 = _table[idx0]; 
        if (i < _npoints - 1)
        {
            int idx1 = idx0 + 1;
            if (idx1 > table_size - 1)
            {
                idx1 = 0;
            }
            float v1 = _table[idx1];  
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
            fprintf(stderr, "error: phase > table size");
            exit(1);
        }

    }
    return true;
}


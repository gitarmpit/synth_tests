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

SignalGen::SignalGen()
{
    _out = 0;
    _npoints = 0;
    for (int i = 0; i < table_size; ++i)
    {
        _table[i] = (float)cos(2.0f * M_PI / (float)table_size * (float)i);
    }
}

SignalGen::~SignalGen()
{
    delete[] _out;
}

bool SignalGen::Generate(int freq, const std::vector<float>& harmAmps, int sps, float duration_sec)
{
    _npoints = (int)(sps * duration_sec);
    if (_out)
    {
        delete[] _out;
    }
    _out = new float[_npoints];
    for (int i = 0; i < _npoints; ++i)
    {
        _out[i] = 0;
    }

    if (harmAmps.size() > maxHarmonics)
    {
        fprintf(stderr, "Too many harmonics. Max=%d\n", maxHarmonics);
    }


    for (size_t nharm = 0; nharm < harmAmps.size(); ++nharm)
    {
        int f = freq * (nharm + 1);
        if (f < sps / 2 && f < 20000)
        {
            if (!GenerateFreq(f, harmAmps[nharm], sps))
            {
                return false;
            }
        }
        else
        {
            fprintf(stderr, "skipping high harmonic: %d\n", f);
        }
    }

    return true;
}

bool SignalGen::GenerateFreq(int freq, float amp, int sps)
{
    float phase = 0.0f;
    bool sin = true;
    if (amp < 0)
    {
        sin = false;
        amp = -amp;
        phase = (float)table_size / (float)4; // 90 degrees
    }

    fprintf (stderr, "freq: %d, sin: %d, amp: %f\n", freq, sin, amp);

    float phase_step = (float)freq / (float)sps * (float)table_size;
    if ((int)phase_step > table_size / 2)
    {
        fprintf(stderr, "error: phase_step too high: %f\n", phase_step);
        return false;
    }

    float val;
    for (int i = 0; i < _npoints; ++i)
    {
        int idx0 = (int)phase;

        float v0 = _table[idx0]; //getPoint(idx0, sin); 
        //float v0 = getPoint(idx0, sin); 
        if (i < _npoints - 1)
        {
            int idx1 = idx0 + 1;
            if (idx1 > table_size - 1)
            {
                idx1 = 0;
            }
            float v1 = _table[idx1]; //getPoint(idx1, sin); 
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
            fprintf(stderr, "error: phase > table size");
            exit(1);
        }

    }
    return true;
}


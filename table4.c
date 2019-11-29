#include <stdio.h>
#include <math.h>
#include <errno.h>
#include "wav.h"
#include <stdlib.h>
#include <string.h>
#include <memory.h>

int main(void) 
{
  const int table_size = 1024; 
  float table[table_size];
  for (int i = 0; i < table_size; ++i) 
  {
    table [ i ] = sin (2.* M_PI / (float)table_size * (float)i);
  }

  int freq = 100;
  int sps = 44100;
  float phase = 0.0f;
  float phase_step = (float) freq / (float)sps * (float)table_size; 

  if ((int)phase_step > table_size / 2) 
  {
     printf ("error: phase_step too high: %7.3f\n", phase_step);
     // exit(1);
  }

  int duration_sec = 1;

  int npoints = sps * duration_sec; 

  short *wav = (short*)malloc (npoints*sizeof(short));
  double amp = 32767;

  fprintf (stderr, "sine freq=%d\n", freq);
  fprintf (stderr, "sample duration, sec:%d\n", duration_sec); 
  fprintf (stderr, "sps: %d\n", sps);
  fprintf (stderr, "table_size=%d\n", table_size); 
  fprintf (stderr, "lookup table phase_step: %f\n", phase_step);
  fprintf (stderr, "total samples in wav: %d\n", npoints);

  short max = 0;
  float val;
  float max_correction = 0;
  int total_corrected = 0;
  for (int i = 0; i < npoints; ++i) 
  {
    int idx0 = (int)phase;

    float v0 = table [idx0];
    if (i < npoints - 1) 
    {
        int idx1 = idx0 + 1;
        if (idx1 > table_size-1) 
        {
          idx1 = 0;
        }
    float v1 = table[idx1];
    float frac = phase - (float)idx0;   
        val = v0 + (v1 - v0) * frac;    
        // fprintf (stderr, "idx0: %d, idx1: %d, phase: %7.3f, frac: %7.3f, v0: %7.3f, v1: %7.3f, corrected val: %7.3f\n", idx0, idx1, phase, frac, v0, v1, val);       
    if (fabs (val - v0) > 0.001) 
        {
           ++total_corrected;  
           // printf ("corrected: v0: %7.3f, val: %7.3f, frac: %7.3f\n", v0, val, frac);
        }
        if (fabs (val - v0) > max_correction) 
        {
      max_correction = fabs (val - v0); 
        }
    }
    else 
    {
       val = v0;
    }


    wav [i] = amp * val;
    phase += phase_step;
    if (phase >= (float)table_size)    
        phase -= (float)table_size;  

    if ((int)phase > table_size - 1) 
    {
       printf ("error: phase > table size\n");
       exit(1);
    }

    if (wav[i] > max) 
    {
      max = wav[i];
    }
  }
  printf ("max amp level (abs, normalized to 16 bit): %d\n", max); 
  printf ("total samples corrected within 0.001 rad: %d\n", total_corrected);
  printf ("max correction, rad: %7.4f (%7.4f deg)\n", max_correction, max_correction*180./M_PI);

  int bps = 16;
  FILE * f = wavfile_open("t.wav", sps, bps);
  if(!f) {
    printf("couldn't open wav for writing: %s",strerror(errno));
    return 1;
  }

  wavfile_write_16(f,wav,npoints);
  wavfile_close(f);
}
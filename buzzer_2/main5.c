#include <errno.h>
#include "buzzer.h" 
#include "singen.h"
#include "wav.h"
#include <stdio.h>

int main(int argc, char**argv) 
{
  int size; 
  int freqs[] = {10};
  float durations[] = {0.2};

  int table_size = 1024;
  int sps = 1000; 
  float phase = 0.0f;
  short amp = 1.; 

  double *buf, *wav = NULL;
  int current_idx = 0;
  size_t length;
  int total_time = 0; 

  for (int i = 0; i < 1; ++i )
  {
     int rc = singen (table_size, sps, freqs[i], &phase, durations[i], amp, &length, &buf);
     if (rc) 
     {
        exit(1);
     }
     wav = (double*)realloc (wav, (current_idx + length) * sizeof(double));
     memcpy (&wav[current_idx], buf, length*sizeof(double));
     free (buf);
     current_idx += length; 
     total_time += durations[i];
  }

  for (int i = 0; i < current_idx; ++i )
  {
    fprintf (stderr, "%d, %f\n", i, wav[i]);
  }

  printf ("total length: %d\n", current_idx); 
  printf ("total time: %d\n", total_time); 
}

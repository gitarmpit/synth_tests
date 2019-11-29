#include <errno.h>
#include "buzzer.h" 
#include "singen.h"
#include "wav.h"
#include <stdio.h>

int main(int argc, char**argv) 
{
  int size; 
  int* freqs;
  float* durations;
  // const char* t = "mars:d=8,o=5,b=2000:16d,d.,16d,4g,4g,4a,4a,4d6.,b,g.";
  
  const char* t = "mars:d=2,o=6,b=200:a";

  if (!convert_tune(t, &size, &freqs, &durations)) 
  {
    exit(1);
  }
  printf ("size: %d\n", size);

  int table_size = 1024;
  int sps = 2000; 
  float phase = 0.0f;
  short amp = 100; 

  short* wav = NULL;
  double* buf;
  int current_idx = 0;
  size_t length;
  int total_time = 0; 

  freqs[0] = 20; 
  freqs[1] = 10; 
  durations[0] = 131;
  durations[1] = 100;


  size = 2;

  for (int i = 0; i < size; ++i )
  {
     if (freqs[i] == 0  )
     {
        continue;
     }
     phase = 0;
     printf ("%d: f=%d, d=%f, phase: %f\n", i, freqs[i], durations[i], phase);
     int rc = singen (table_size, sps, freqs[i], &phase, durations[i]/1000.0f, amp, &length, &buf);
     if (rc) 
     {
        exit(1);
     }
     printf ("length: %d\n", length);
     wav = (short*)realloc (wav, (current_idx + length) * sizeof(short));
     short* sbuf = (short*)malloc (length * sizeof(short));
     for (int b = 0; b < length; ++b)
     {
        sbuf[b] = (short)buf[b];
     }
     memcpy (&wav[current_idx], sbuf, length*sizeof(short));
     free (buf);
     free (sbuf);
     current_idx += length; 
     total_time += durations[i];
  }

  for (int i = 0; i < current_idx; ++i )
  {
    fprintf (stderr, "%d, %d\n", i, wav[i]);
  }

  printf ("total length: %d\n", current_idx); 
  printf ("total time: %d\n", total_time); 

  int bps = 16;
  FILE * f = wavfile_open("t.wav", sps, bps);
  if(!f) {
    printf("couldn't open wav for writing: %s",strerror(errno));
    return 1;
  }

  wavfile_write_16(f,wav,current_idx);
  wavfile_close(f);

   

}

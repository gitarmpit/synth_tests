#include <errno.h>
#include "buzzer.h" 
#include "SignalGen.h"
#include "wav.h"
#include <stdio.h>

int main(int argc, char**argv) 
{
  int size; 
  int* freqs;
  float* durations;
  //const char* t = "mars:d=8,o=5,b=200:16d,d.,16d,4g,4g,4a,4a,4d6.,b,g.";
  //const char* t = "barbie:d=8,0=4,b=250:g#,e,g#,c#6,4a,4p,f#,d#,f#,b,4g#";
  const char* t = "nokia:d=8,o=5,b=220:e6,d6,4f#,4g#,c#6,b,4d,4e,b,a,4c#,4e,1a";
  //const char* t = argv[1];

  // const char* t = "mars:d=8,o=2,b=2000:1d,1e";
  
  // const char* t = "mars:d=2,o=2,b=2000:a,c,c3";

  if (!convert_tune(t, &size, &freqs, &durations)) 
  {
    exit(1);
  }
  
  // printf ("size: %d\n", size);

  int sps = 48000; 
  double phase = 0.0f;
  short amp = 10000; 

  short* wav = NULL;
  double* buf;
  int current_idx = 0;
  int length;
  int total_time = 0; 

  SignalGen signalGen(sps, true);
  // Generate(int freq, const std::vector<double>& harmAmps, const std::vector<double>& phases, double duration_sec)


  for (int i = 0; i < size; ++i )
  {
     if (freqs[i] < 0)
     {
        amp = 0;
        freqs[i] = -freqs[i];
     }
     else 
     {
        amp = 10000;
     }
     // printf ("%d: f=%d, d=%f, phase: %f\n", i, freqs[i], durations[i], phase);
     // int rc = singen (table_size, sps, freqs[i], &phase, durations[i]/1000.0f, amp, &length, &buf);
     // phase = 0;
     if (!signalGen.GenerateOne (freqs[i], amp, phase, durations[i]/1000.0)) 
     {
        printf ("error\n");
        exit(1);
     }

     signalGen.Get(length, buf);
     wav = (short*)realloc (wav, (current_idx + length) * sizeof(short));
     short* sbuf = (short*)malloc (length * sizeof(short));
     for (int b = 0; b < length; ++b)
     {
        wav[current_idx + b] = (short)buf[b];
        printf ("%d, %f\n", current_idx + b, buf[b]);
     }

     free (sbuf);
     current_idx += length; 
     total_time  += durations[i];
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

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
  const char* t = "barbie:d=8,0=4,b=160:g#,e,g#,c#6,4a,4p,f#,d#,f#,b,4g#";
  //const char* t = "nokia:d=8,o=5,b=220:e6,d6,4f#,4g#,c#6,b,4d,4e,b,a,4c#,4e,1a";
  //const char* t = "pv:d=8,o=5,b=120:c,eb,g,eb,4f,eb,d,4g,4f,2c,eb,g,bb,bb,4c6,bb,ab,2g,4a,4b,d6,c6,4g.,4d,c,g.,16f,2ab,bb,ab,4g,f,eb,4g,4f,2c";
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

  SignalGen signalGen(sps, 0);
  // Generate(int freq, const std::vector<double>& harmAmps, const std::vector<double>& phases, double duration_sec)

  std::vector<double> harmAmps;
  std::vector<double> phases;
  harmAmps.push_back (10000);
  harmAmps.push_back (0);
  harmAmps.push_back (10000);
  phases.push_back (0);
  phases.push_back (0);
  phases.push_back (0);

  for (int i = 0; i < size; ++i )
  {
     printf ("%d: f=%d, d=%f, phase: %f, amp: %d\n", i, freqs[i], durations[i], phase, amp);
     if (!signalGen.Generate (freqs[i], harmAmps, phases, durations[i]/1000.0)) 
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
        //printf ("%d, %f\n", current_idx + b, buf[b]);
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

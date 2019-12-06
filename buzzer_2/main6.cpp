#include <errno.h>
#include "buzzer.h" 
#include "singen.h"
#include <stdio.h>
#include "SignalGen.h"
#include "wav.h"


int main(int argc, char**argv) 
{
  int freqs[] = {10, 30, 10, 30, 20};
  float durations[] = {.33, .26, .34, .41, 32};
  short amps[] = {20,20,20,20,20};

  int sps = 300; 
  double phase = 0.0f;

  double *buf, *wav = NULL;
  int total_length = 0;
  int total_time = 0; 
  short* swav = NULL;

  SignalGen signalGen(sps);
//  goto err; 
  for (int i = 0; i < 4; ++i )
  {
     phase = 0;
     if (!signalGen.GenerateOne(freqs[i], amps[i], phase, durations[i], false)) 
     {
        exit(1);
     }

      //int length = 10000000;
     int length;
     //buf = (double*)malloc (length * sizeof(buf));
     signalGen.Get(length, buf);
     
     wav = (double*)realloc (wav, (total_length + length) * sizeof(double));
     memcpy (&wav[total_length], buf, length*sizeof(double));
     total_length += length; 
     total_time += durations[i];
  }


  swav = (short*) malloc (total_length * sizeof(short));

  for (int i = 0; i < total_length; ++i )
  {
    fprintf (stderr, "%d, %f\n", i, wav[i]);
    //fprintf (stderr, "%.18g\n", wav[i]);
    if (int(wav[i] > 32767)) 
    {
      printf ("clipping: %d: %d\n", i, (int)wav[i]);
    }
    swav[i] = (short)wav[i];
  }

  //exit(1);
  // printf ("total length: %d\n", total_length); 
  // printf ("total time: %d\n", total_time); 

  int bps = 16;
  FILE * f = wavfile_open("t.wav", sps, bps);
  if(!f) {
    printf("couldn't open wav for writing: %s",strerror(errno));
    return 1;
  }

  wavfile_write_16(f,swav,total_length);
  wavfile_close(f);

}

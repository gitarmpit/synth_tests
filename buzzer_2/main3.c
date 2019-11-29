#include <errno.h>
#include "buzzer.h" 
#include "singen.h"
#include "wav.h"
#include <stdio.h>

int main(int argc, char**argv) 
{
  int size; 
  int table_size = 1024;
  int sps = 4000; 
  float phase = 0.0f;
  short amp = 10000; 
  int freq = 100; 

  double* buf;
  size_t length;

  int rc = singen (table_size, sps, freq, phase, 2, amp, &length, &buf);

  short* wav = (short*)malloc (length * sizeof(short));
  for (int i = 0; i < length; ++i)
  {
     wav[i] = (short)buf[i];
  }

  int bps = 16;
  FILE * f = wavfile_open("t.wav", sps, bps);
  if(!f) {
    printf("couldn't open wav for writing: %s",strerror(errno));
    return 1;
  }

  wavfile_write_16(f,wav,length);
  wavfile_close(f);
   

}

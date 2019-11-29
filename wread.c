#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "wav.h"


int main(void) 
{
  struct wavfile_header header;
  FILE * f = fopen("t.wav", "r");
  if (!f) 
  {
     printf("couldn't open wav for reading: %s",strerror(errno));
     exit(1);
  }

  if ( fread(&header, sizeof(header),1 , f) != 1)
  {
     printf("reading err: %s",strerror(errno));
     exit(1);
  }

  int nsamples = header.data_length / 2;  //two bytes per sample

  short* data = (short*)malloc (nsamples * sizeof(short));

  printf ("data len: %d\n", nsamples);
  fread(data, nsamples*sizeof(short),1 , f);

  for (int i = 0; i < nsamples; ++i )
  {
     printf ("%d\n", data[i]);
  }

  exit(0);


}
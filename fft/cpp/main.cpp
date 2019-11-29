#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "FFT.h"
#include "wav.h"


FILE * wavfile_open( const char *filename, int  samples_per_second, int bits_per_sample)
{
	struct wavfile_header header;

	// int samples_per_second = WAVFILE_SAMPLES_PER_SECOND;
	// int bits_per_sample = 16;

        if (bits_per_sample != 16 && bits_per_sample != 24 && bits_per_sample != 32) 
        {
           printf ("16/24/32 bps only\n");
           exit(1);
        }

	strncpy(header.riff_tag,"RIFF",4);
	strncpy(header.wave_tag,"WAVE",4);
	strncpy(header.fmt_tag,"fmt ",4);
	strncpy(header.data_tag,"data",4);

	header.riff_length = 0;
	header.fmt_length = 16;
	header.audio_format = 1; // PCM
	header.num_channels = 1;
	header.sample_rate = samples_per_second;
	header.byte_rate = samples_per_second*(bits_per_sample/8);
	header.block_align = bits_per_sample/8;
	header.bits_per_sample = bits_per_sample;
	header.data_length = 0;

	FILE * file = fopen(filename,"wb+");
	if(!file) return 0;

	fwrite(&header,sizeof(header),1,file);

	fflush(file);

	return file;

}

void wavfile_write_16( FILE *file, short data[], int length )
{
	fwrite(data,sizeof(short),length,file);
}

void wavfile_write_32( FILE *file, int data[], int length )
{
	fwrite(data,sizeof(int),length,file);
}


void wavfile_close( FILE *file )
{
	int file_length = ftell(file);

	int data_length = file_length - sizeof(struct wavfile_header);
	fseek(file,sizeof(struct wavfile_header) - sizeof(int),SEEK_SET);
	fwrite(&data_length,sizeof(data_length),1,file);

	int riff_length = file_length - 8;
	fseek(file,4,SEEK_SET);
	fwrite(&riff_length,sizeof(riff_length),1,file);

	fclose(file);
}


int main(void) 
{
  const int table_size = 1024; 
  float table[table_size];
  for (int i = 0; i < table_size; ++i) 
  {
    table [ i ] = sin (2.* M_PI / (float)table_size * (float)i);
  }

  bool logAxis = false;
  int width = 400;
  int window_size = 2048;
  int freq = 11384;
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

  float *wav = (float*)malloc (npoints*sizeof(float));
  short *wavs = (short*)malloc (npoints*sizeof(short));

  fprintf (stderr, "sine freq=%d\n", freq);
  fprintf (stderr, "sample duration, sec:%d\n", duration_sec); 
  fprintf (stderr, "sps: %d\n", sps);
  fprintf (stderr, "table_size=%d\n", table_size); 
  fprintf (stderr, "lookup table phase_step: %f\n", phase_step);
  fprintf (stderr, "total samples in wav: %d\n", npoints);

  float val;
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
    }
    else 
    {
       val = v0;
    }

    short w = 32767. * val;

    wav [i] = (double)w / (double) 32767;
    // printf ("%d: %7.4f\n", i, wav[i]);

    
    wavs [i] = w;
    phase += phase_step;
    if (phase >= (float)table_size)    
        phase -= (float)table_size;  

    if ((int)phase > table_size - 1) 
    {
       printf ("error: phase > table size\n");
       exit(1);
    }

  }

  /**

  int bps = 16;
  FILE * f = wavfile_open("t.wav", sps, bps);
  if (!f) 
  {
     printf("couldn't open wav for writing: %s",strerror(errno));
     return 1;
  }

  wavfile_write_16(f,wavs,npoints);
  wavfile_close(f);
  printf ("all done\n");
  */

  //FreqWindow fw(3, SpectrumAnalyst::Spectrum, 44100., 10000, 1024);
  FFT fft;
  // eWinFuncBlackmanHarris is the narrowest
  // Then BlackMan then Hann them Hamming is really wide
  fft.Run(eWinFuncBlackmanHarris, (float)sps, window_size, npoints, wav);
  fft.CalculateView(width, 400, logAxis);
  exit(0);


}
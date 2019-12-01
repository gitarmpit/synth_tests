#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include "SignalGen.h"
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


// Format of harmonics file:  
//  frequency,phase  : one per line 

int main(int argc, char**argv) 
{
    if (argc < 5)
    {
        fprintf(stderr, "args: <sps> <freq> <sec> <amp> [ <file with harms> ]");
        exit(1);
    }

    int sps = atoi(argv[1]);
    int freq = atoi(argv[2]);
    float sec = atof(argv[3]);
    float amp = atof(argv[4]);

    char* fharms = NULL;
   
    if (argc == 6) 
    {
       char* fharms = _strdup(argv[5]);
    }

    fprintf(stderr, "sps: %d, freq: %d, amp: %f, sec: %f\n", sps, freq, amp, sec);

    if (sps < 1000 || sps >44100)
    {
        fprintf(stderr, "sps must be in 1000:44100 range");
        exit(1);
    }

    if (freq >= sps / 2)
    {
        fprintf(stderr, "freq must be < sps/2 (%d)\n", sps/2);
        exit(1);
    }

    if (sec < 0.1 || sec > 200)
    {
        fprintf(stderr, "sec must be in 0.1 to 200 sec range\n");
        exit(1);
    }

    std::vector<float> harms; 
    std::vector<float> phases; 

    if (fharms != NULL) 
    {
    char buf[256];
    	FILE* fp = fopen(fharms, "r");
    if (fp == NULL)
    {
        perror (fharms);
        exit(1);
    }

    
    float h;
    while (fgets(buf, sizeof buf, fp))  
    {       
    	char* token = strtok(buf, ",");
        float phase = 0.0f;
    	if (token != NULL) 
    	{
           h = atof(token);
           token = strtok(NULL, ",");
           if (token != NULL)
           {
              phase = atof(token);
           }
        }
        else 
        {
           h = atof(buf);
        }

        fprintf (stderr, "hamp: %f, phase: %f\n", h, phase);
        if (h < 0.0001 && h > 10000) 
        {
           fprintf(stderr, "amp range must be 0.0001 to 10000\n");
           exit(1);
        }
        if (abs(phase) > 180) 
        {
           fprintf (stderr, "phases must be within -= 180 degrees\n");
           exit(1);
        }
   
        harms.push_back(h*amp);
        phases.push_back(phase*M_PI/180);
    }
 
    fclose(fp);

    if (harms.size() == 0)
    {
        fprintf(stderr, "no harms, using single freq: %d\n", freq);
        harms.push_back (1.);
        phases.push_back(0.);
    }
 
    SignalGen signalGen(sps); 
    if (!signalGen.Generate(freq, harms, phases, sec)) 
    {
        exit(1);
    }
    float* res;
    int size = 0;
    signalGen.Get(size, res);
  
    short* wav = (short*)malloc(size * sizeof(short));

    for (int i = 0; i < size; ++i)
    {
        wav[i] =  (short)res[i];
        printf("%d, %f\n", i, res[i]);
    }

    int bps = 16;
    FILE* f = wavfile_open("t.wav", sps, bps);
    if (!f)
    {
        printf("couldn't open wav for writing: %s", strerror(errno));
        return 1;
    }

    wavfile_write_16(f, wav, size);
    wavfile_close(f);

    free(wav);


}
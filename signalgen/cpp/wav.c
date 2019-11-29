#include "wav.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct wavfile_header {
	char	riff_tag[4];
	int	riff_length;
	char	wave_tag[4];
	char	fmt_tag[4];  //12
	int	fmt_length;  //16     16
	short	audio_format;  //20   1 = PCM
	short	num_channels;  //22
	int	sample_rate;   //24
	int	byte_rate;     //28   SampleRate * NumChannels * BitsPerSample/8
	short	block_align;   //32   NumChannels * BitsPerSample/8
	short	bits_per_sample; //34 
	char	data_tag[4];     //36 "data"
	int	data_length;     //40 NumSamples * NumChannels * BitsPerSample/8
};

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

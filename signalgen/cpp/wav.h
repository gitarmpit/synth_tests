#ifndef WAVFILE_H
#define WAVFILE_H

#include <stdio.h>
#include <inttypes.h>

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


#endif

#ifndef __SINGEN_H 
#define __SINGEN_H

#include <inttypes.h>
#include <stdio.h> 
#include <math.h>
#include <stdlib.h>


int singen (int table_size, int sps, int freq, float *phase, float duration_sec, short amp, size_t* length, double** out);

#endif
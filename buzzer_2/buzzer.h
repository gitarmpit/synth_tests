#ifndef __BUZZER_H 
#define __BUZZER_H

#include <stddef.h> 
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

typedef int BOOL;
#define TRUE 1
#define FALSE 0


BOOL convert_tune(const char* tune, int* size, int** freqs, float** durations);


#endif
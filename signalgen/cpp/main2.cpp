#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include "SignalGen.h"

int main(int argc, char**argv) 
{
    if (argc != 5)
    {
        fprintf(stderr, "args: <freq> <amp> <phase> <dur sec>");
        exit(1);
    }

    int freq = atoi(argv[1]);
    float amp = atof(argv[2]);
    float phase = atof(argv[3]);
    float sec = atof(argv[4]);

    phase = phase*M_PI/180;
    
    SignalGen signalGen(4000); 
    if (!signalGen.GenerateOne(freq, amp, phase, sec)) 
    {
        exit(1);
    }
    float* res;
    int size = 0;
    signalGen.Get(size, res);
  
    for (int i = 0; i < size; ++i)
    {
        printf("%d, %f\n", i, res[i]);
    }

}
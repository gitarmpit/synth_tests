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
    if (argc != 6)
    {
        fprintf(stderr, "args: <sps> <freq> <amp> <phase> <dur sec>");
        exit(1);
    }

    float sps = atof(argv[1]);
    int freq = atoi(argv[2]);
    float amp = atof(argv[3]);
    float phase = atof(argv[4]);
    float sec = atof(argv[5]);

    phase = phase*M_PI/180;
    
    SignalGen signalGen(sps); 
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
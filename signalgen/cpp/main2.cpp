#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include "SignalGen.h"
#include <unistd.h>

int main(int argc, char**argv) 
{
    int c;
    int sps = 0;
    int freq = 0;
    double amp = 1;
    double phase = 0;
    double sec = 0;
    double step = -1;

    while ((c = getopt (argc, argv, "S:s:a:f:p:d:")) != -1)
    {
      switch (c)
      {
        case 's':
          sps = atof(optarg);
          break;
        case 'S':
          step = atof(optarg);
          break;
        case 'a':
          amp = atof(optarg);
          break;
        case 'f':
          freq = atoi(optarg);
          break;
        case 'p':
          phase = atof(optarg);
          break;
        case 'd':
          sec = atof(optarg);
          break;
 
        default:
          fprintf (stderr, "unexpected option: %c", c);
          exit(1);
      }
    }

    if (step > 0) 
    {
      sps = (int)(sec/step + 0.5);
    }

    if (sps < 10) 
    {
      fprintf (stderr, "sps has to be set to >10\n");
      exit(1);
    }

    if (freq == 0) 
    {
      fprintf (stderr, "freq not set\n");
      exit(1);
    }

    if (freq >= sps/2) 
    {
      fprintf (stderr, "sps too low: %d\n", sps);
      exit(1);
    }

    phase = phase*M_PI/180;

    fprintf (stderr, "freq: %d, phase: %f, amp: %f, sps: %d, sec: %f\n", 
             freq, phase, amp, sps, sec);
    
    SignalGen signalGen(sps); 
    if (!signalGen.GenerateOne(freq, amp, phase, sec)) 
    {
        exit(1);
    }
    double* res;
    int size = 0;
    signalGen.Get(size, res);

    for (int i = 0; i < size; ++i)
    {
        printf("%.18f\n", res[i]);
    }

}
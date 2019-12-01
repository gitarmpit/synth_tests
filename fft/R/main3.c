#include <stdio.h>
#include <stdlib.h>
#include "fft.h"

int main (int argc, char**argv) 
{
  
   double re[] = { 1.000000,  0, 0.809017,  0, 0.309017, 0, -0.309017, 0, -0.809017, 0, -1.000000, 0, -0.809017, 0,
                 -0.309017,  0, 0.309017,  0, 0.809017,  0};

   int length = sizeof re / sizeof(double) / 2; 

   double* a = (double*)malloc (length * sizeof(double)); 
   double* p = (double*)malloc (length * sizeof(double)); 
   if (!fft_complex (re, length, a, p)) 
   {
      fprintf (stderr, "error\n");
      exit(1);
   }

   for (int i = 0; i < length; ++i)
   {
       printf("%d: amp:%f ph:%f\n", i, a[i], p[i]);
   }

}
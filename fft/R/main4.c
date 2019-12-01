#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fft.h"

int main (int argc, char**argv) 
{
  
   double re[] = { 1.000000, 0.809017,  0.309017, -0.309017, -0.809017, -1.000000, -0.809017, 
                 -0.309017,  0.309017,  0.809017, 1.000000 };

   int length = sizeof re / sizeof(double); 
   double* a = (double*)malloc (length * sizeof(double)); 
   double* p = (double*)malloc (length * sizeof(double)); 
   if (!fft (re, length, a, p)) 
   {
      fprintf (stderr, "error\n");
      exit(1);
   }

   for (int i = 0; i < length; ++i)
   {
       printf("%d: amp:%f ph:%f\n", i, a[i], p[i]);
   }


}
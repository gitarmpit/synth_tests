#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fft.h"

int main (int argc, char**argv) 
{

   const int sps = 100;
   double re[sps]; 
   double f = 10;
   for (int i = 0; i < sps; ++i) 
   {
     double t = (double)i / sps; 
     re[i] = cos (2*M_PI*f*t);
     printf ("%d: %f\n", i, re[i]);
   }  

   int length = sizeof re / sizeof(double); 
   double* a = (double*)malloc (length * sizeof(double)); 
   double* p = (double*)malloc (length * sizeof(double)); 
   if (!fft (re, length, a, p)) 
   {
      fprintf (stderr, "error\n");
      exit(1);
   }

   for (int i = 0; i < length/2; ++i)
   {
       printf("%d: amp:%f ph:%f\n", i, a[i], p[i]);
   }


}
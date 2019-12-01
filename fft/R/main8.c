#include <stdio.h>
#include <stdlib.h>
#include "fft.h"
#include <math.h>

int main (int argc, char**argv) 
{
   //double d = M_PI/1000000000000.;
   //printf ("g: %g, e: %e,  18g: %.18g, 18e: %.18e\n", d,d,d,d);
   //exit(1);

   double f = 32;
   double step = M_PI/1000;
   int len = (int) (1 / step + 1);
   double re[len*2]; 
   printf ("estimated cnt: %d\n", len);

   len = 0; 
   double t = 0;
   do 
   {
     re[len] = 137*cos (2*M_PI*f*t + M_PI/3);
     re[len+1] = 0;
     // printf ("%d: t=%f %f\n", len/2, t, re[len]);
     t += step;
     len += 2;
   } while (t < 0.999);

   len /= 2;
   printf ("actual len: %d\n", len);

   double* a = (double*)malloc (len/2 * sizeof(double)); 
   double* p = (double*)malloc (len/2 * sizeof(double)); 
   if (!fft_complex (re, len, a, p)) 
   {
      fprintf (stderr, "error\n");
      exit(1);
   }

   for (int i = 0; i < len/2; ++i)
   {
       printf("%d: amp:%.8g ph:%.8g\n", i, a[i]/len*2, p[i]);
   }

}
#include <stdio.h>
#include <stdlib.h>
#include "fft.h"
#include <math.h>

int main (int argc, char**argv) 
{
  
   const int sps = 10;
   double* re = (double*)calloc (sps*2, sizeof(double)); 
   double f = 1;
   for (int i = 0; i < sps; ++i) 
   {
     double t = (double)i / sps; 
     re[i*2] = cos (2*M_PI*f*t);
     printf ("%d: %f\n", i, re[i*2]);
   }  

   int length = 11; 

   int maxf, maxp;   
   BOOL rc2 = fft_factor(length, &maxf, &maxp);
   if (!rc2)
   {     
      printf ("factor error\n");
      exit(1);
   }  

   int inv = -2;
   fprintf (stderr, "temporal, inv=%d, n=%d, work alloc: %u, iwork alloc: %d\n", 
           inv, length, 4 * maxf * sizeof(double), maxp* sizeof(int));
  
   printf ("max factor: %d, scratch storage required: %d\n", maxf, maxp);
   double* work = (double*)malloc(4 * maxf * sizeof(double));
   int*   iwork = (int*)malloc(maxp * sizeof(int));
   rc2 = fft_work(re, &re[1], 1, length, 1, inv, work, iwork);

   printf ("rc = %d\n", rc2);
   for (int i = 0; i < length*2; i += 2)
   {
       printf("%d: %f, %f\n", i/2, re[i], re[i+1]);
   }


}
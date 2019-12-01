#include <stdio.h>
#include <stdlib.h>
#include "fft.h"

int main (int argc, char**argv) 
{
  
   double re[] = { 1.000000,  0, 0.809017,  0, 0.309017, 0, -0.309017, 0, -0.809017, 0, -1.000000, 0, -0.809017, 0,
                 -0.309017,  0, 0.309017,  0, 0.809017,  0};

   int length = sizeof re / sizeof(double) / 2; 

   int maxf, maxp;   
   BOOL rc2;
   fft_factor(length, &maxf, &maxp);
   //if (!rc2)
   //{     
   //   printf ("factor error\n");
   //   exit(1);
   //}  

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

   for (int i = 0; i < len; ++i)
      {
        printf ("%d: %f %fi\n", i, re[i], im[i]);
       	amps[i] = sqrt(re[i]*re[i] + im[i]*im[i]);
       	phases[i] = atan2(im[i], re[i]);
      }

}
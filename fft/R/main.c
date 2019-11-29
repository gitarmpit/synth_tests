#include <stdio.h>
#include <stdlib.h>
#include "fft.h"
#include "singen.h" 

int main (int argc, char**argv) 
{

   int table_size = 1024;
   int sps = 44100; 
   int freq = 1307; 
   float phase = 0.0f;
   float duration_sec = 1.0f;
   short amp = 1000; 
   size_t length; 
   double* out;
   int rc = singen (table_size, sps, freq, phase, duration_sec, amp, &length, &out);
   if (rc) 
   {
      exit(1);
   }


    /*
   for (size_t i = 0; i < length; ++i) 
   {
      printf ("%d: %f\n", i, out[i]);
   }
   */

   printf ("size: %d\n", length);

   int maxf, maxp;   
   BOOL rc2 = fft_factor(length, &maxf, &maxp);
   if (!rc2)
   {     
      printf ("factor error\n");
      exit(1);
   }  
  
   printf ("max factor: %d, scratch storage required: %d\n", maxf, maxp);
   double* work = (double*)malloc(4 * maxf * sizeof(double));
   int*   iwork = (int*)malloc(maxp * sizeof(int));
   int inv = 1;
   double* out2 = (double*)calloc (length, sizeof (double));
   rc2 = fft_work(out2, out, 1, length, 1, inv, work, iwork);

   printf ("rc = %d\n", rc2);
   for (int i = 0; i < length; ++i)
   {
       printf("%d: %f\n", i, out[i]);
   }


}
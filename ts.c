#include <math.h> 
#include <stdio.h> 
#include <stdlib.h>

static double fact[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880,  3628800 };
static double c_vals[] = { 1, 0, -1, 0, 1, 0, -1, 0, 1, 0, -1 };

void ts_cos (double a, double f, double n, double* out) 
{
   if (n < 0 || n >= 10 )
   {
      n = 10;
   }

   for (int i = 0; i <= 10; ++i)
   {
      out [i] = fact[i] * c_vals[i];
   }

   return; 
}

int main(int argc, char**argv) 
{
   double out[10];

   ts_cos (1, 1, 10, out);

   for (int i = 0; i <= 10; ++i)
   {
      // printf ("%f ", out[i]);
   }

   //printf ("\n");
   //exit(1);

   for (double t = -M_PI*1.2; t <= M_PI*1.2; t += 0.1)
   {
      double v = out[0] + pow (t, 2)/out[2] + pow (t, 4)/out[4] + pow (t, 6)/out[6] + pow (t, 8)/out[8] + pow (t, 10)/out[10];
      printf ("%f, %f\n", t, v);
   }
      
}

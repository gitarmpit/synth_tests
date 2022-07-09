#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double curvee(double x) 
{
  return pow(fabs(fmod (fabs(x), M_PI*2) - M_PI), 2) / M_PI / M_PI * 2 - 1;
}

double curvee2(double x) 
{
  return pow(fabs(fmod (fabs(x), M_PI*2) - M_PI), 0.5) / sqrt(M_PI) * 2 - 1;
}

double triangle(double x)
{
   return fabs(fmod (fabs(x), M_PI*2) - M_PI) / M_PI * 2 - 1;
}

double sawtooth (double x) 
{
   return -(fabs(fmod (fabs(x), M_PI*2)) / M_PI - 1);
}

/*
float sawtooth (float x) 
{
   float m = fmod (x - M_PI/2, M_PI*2);
   if (m < 0) 
     m = M_PI*2 + m;
   return (fabs(m) / M_PI - 1);
}
*/

double square (double x) 
{
   return fabs (fmod (fabs(x), M_PI*2) < M_PI ? 1 : 0) - 0.5;
}

double square2 (double x) 
{
   static const double scale = M_PI/4;
   static double thr1 = M_PI - scale;
   static double thr2 = 2*M_PI - scale;

   double phase = fmod (fabs(x), M_PI*2);

   if (phase >= thr1 && phase <= M_PI) 
   {
      return triangle (M_PI/scale*(phase-thr1));
   }
   else if (phase >=thr2) 
   {
     return triangle (M_PI + M_PI/scale*(phase-thr2));
   }
   else 
   {
     return fabs(   phase < M_PI ? 2 : 0  ) - 1;
   }
}

/*
float square2 (float x) 
{
   static const float scale = M_PI/3;
   static float thr1 = M_PI - scale;
   static float thr2 = 2*M_PI - scale;

   //float phase = fmod(x+M_PI/2-scale/2, M_PI*2);
   float phase = fmod(x+M_PI/2, M_PI*2);
   if (phase < 0) 
     phase = M_PI*2 + phase;

   if (phase >= thr1 && phase <= M_PI) 
   {
      return triangle (M_PI/scale*(phase-thr1));
   }
   else if (phase >=thr2) 
   {
     return triangle (M_PI + M_PI/scale*(phase-thr2));
   }
   else 
   {
       return phase < M_PI ? 1 : -1;
   }
}
*/

/*

double square_odd (double x) 
{
   float m = fmod(x, M_PI*2);
   if (m < 0) 
     m = M_PI*2 + m;
   return m < M_PI ? 1 : -1;
}
*/

double square_odd (double x) 
{
    return (x < 0) ? 0 : 1;
}


double simps3 (double a, double b, double(*f)(double)) 
{
    return f(a) + 4*f((a + b) / 2) + f(b);
}


double calc_simps (double a, double b, double(*f)(double)) 
{
    double dx = M_PI/1000;

    int nsteps = ceil (fabs(a - b) / dx) + 1; 

    double p = a;
    double result3 = 0; 
    for (int i = 0; i < nsteps; ++i)
    {
       result3 += simps3 (p, p+dx, f);
       p += dx; 
    }

    return dx/6 * result3;
}

double _T,  _n;
double(*_fptr)(double);

double afunc (double x) 
{
  return _fptr(x)*cos(2*M_PI/_T*_n*x);
}

double bfunc (double x) 
{
  return _fptr(x)*sin(2*M_PI/_T*_n*x);
}


int main(void) 
{
   _T = 2*M_PI;

   /*
   for (float i = -360*2; i <= 360*2; i += 10) 
   {
      printf ("%f, %f\n", i, triangle(M_PI*(float)i/180.0f));
   }

   exit(1);
   */

   /*
   for (int i = 0; i < n; ++i) 
   {
      float t = (float)i / sps;
      // printf ("%f, %f\n", t, triangle(2*M_PI*f*t)); 
      printf ("%f, %f\n", t, square(2*M_PI*f*t)); 
   }

   exit(1);
   */


   _fptr = square_odd;
   double nstep = .1;
   for (_n = -16; _n < 16; _n += nstep) 
   {
      float a = 2/_T * calc_simps (-_T/2, _T/2, afunc);
      float b = 2/_T * calc_simps (-_T/2, _T/2, bfunc);
      float amp = sqrt(a*a + b*b);
      float phase = atan2(b, a)*180/M_PI;
      if (amp < 0.00001) 
      {
         //amp = 0; 
         //phase = 0;
      }
      if (fabs(fabs(phase) - 180) < 0.01) 
      {
         //phase = 0; 
         //amp = -amp;
      }
      //printf ("%f,%f\n", _n, 10*log(amp));
      printf ("%f,%f\n", _n, amp);
      //printf ("%f: a: %f, b: %f, amp: %f, phase: %f\n", _n, a, b, amp, phase);
   }
}

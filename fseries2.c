#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float curvee(float x) 
{
  return pow(fabs(fmod (fabs(x), M_PI*2) - M_PI), 2) / M_PI / M_PI * 2 - 1;
}

float curvee2(float x) 
{
  return pow(fabs(fmod (fabs(x), M_PI*2) - M_PI), 0.5) / sqrt(M_PI) * 2 - 1;
}

float triangle(float x)
{
   return fabs(fmod (fabs(x), M_PI*2) - M_PI) / M_PI * 2 - 1;
}

float sawtooth (float x) 
{
   return -(fabs(fmod (fabs(x), M_PI*2)) / M_PI - 1);
}

float square (float x) 
{
   return fabs (fmod (fabs(x), M_PI*2) < M_PI ? 1 : 0) - 0.5;
}

float square2 (float x) 
{
   static const float scale = M_PI/10;
   static float thr1 = M_PI - scale;
   static float thr2 = 2*M_PI - scale;

   float phase = fmod (fabs(x), M_PI*2);

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

float stcos(float t, float f, float n, float(*st)(float))
{
   return st(t) * cos (2.0f * M_PI * f * n * t);
}

float stsin(float t, float f, float n, float(*st)(float))
{
   return st(t) * cos (2.0f * M_PI * f * n * t);
}

float calc_midpoint_rule(float a, float b, float n, float(*f)(float) )
{
  float dx = (b - a) / n;
  float sum = 0;
  for (int i = 0; i < n; ++i)
  {
    float p1 = dx * (float)i;
    float p2 = p1 + dx;
    float x = a + (p1 + p2) / 2.;
    sum += f(x) * dx;
  }

  return sum;
}


float _T;
float(*_fptr)(float);
int _n;

float afunc (float x) 
{
  return _fptr(x)*cos(2*M_PI*1/_T*_n*x);
}

float bfunc (float x) 
{
  return _fptr(x)*sin(2*M_PI*1/_T*_n*x);
}


int main(void) 
{
/* 
   float f = 10;
   int sps = 100;
   int dur = 2;
   int n = sps * dur;

   for (float i = -360*2; i <= 360*2; i += 0.1) 
   {
      printf ("%f, %f\n", i, curvee2(M_PI*(float)i/180.0f));
   }

   exit(1);
   
   for (int i = 0; i < n; ++i) 
   {
      float t = (float)i / sps;
      // printf ("%f, %f\n", t, triangle(2*M_PI*f*t)); 
      printf ("%f, %f\n", t, square(2*M_PI*f*t)); 
   }

   exit(1);
*/

   // float(*fptr)(float) = triangle;
   _T = 2*M_PI;
   //float f = 1/T;
   float c = calc_midpoint_rule (-_T/2, _T/2, 10000, triangle);

   _fptr = triangle;
   printf ("c: %f\n", c);
   float a[30], b[30];
   for (_n = 1; _n <= 30; ++_n) 
   {
      a[_n] = 2/_T * calc_midpoint_rule (-_T/2, _T/2, 10000, afunc);
      b[_n] = 2/_T * calc_midpoint_rule (-_T/2, _T/2, 10000, bfunc);
      //printf ("n: %d, a: %f, b: %f\n", _n, a[_n], b[_n]);
      printf ("%f,", a[_n]);
   }



}
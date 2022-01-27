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
   float m = fmod (x - M_PI/2, M_PI*2);
   if (m < 0) 
     m = M_PI*2 + m;
   return (fabs(m) / M_PI - 1);
}


float square (float x) 
{
   float m = fmod(x+M_PI/2, M_PI*2);
   if (m < 0) 
     m = M_PI*2 + m;
   return m < M_PI ? 1 : -1;
}

float square_odd (float x) 
{
   float m = fmod(x, M_PI*2);
   if (m < 0) 
     m = M_PI*2 + m;
   return m < M_PI ? 1 : -1;
}


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
float _n;

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
 
   float f = 10;
   int sps = 100;
   int dur = 2;
   int n = sps * dur;

   for (float i = -360*2; i <= 360*2; i += 10) 
   {
      printf ("%f, %f\n", i, triangle(M_PI*(float)i/180.0f));
   }

   exit(1);
   
   /*
   for (int i = 0; i < n; ++i) 
   {
      float t = (float)i / sps;
      // printf ("%f, %f\n", t, triangle(2*M_PI*f*t)); 
      printf ("%f, %f\n", t, square(2*M_PI*f*t)); 
   }

   exit(1);
   */

   _fptr = triangle;
   _T = 2*M_PI;
   float c = calc_midpoint_rule (0, _T, 1000, _fptr);

   //printf ("c: %f\n", c);
   
   const int sz = 20;
   //float a[sz], b[sz];
   float nstep = .1;
   for (_n = -20; _n < 20; _n += nstep) 
   {
      float a = 2/_T * calc_midpoint_rule (-_T/2, _T/2, 100000, afunc);
      float b = 2/_T * calc_midpoint_rule (-_T/2, _T/2, 100000, bfunc);
      //printf ("n: %d, a: %7.4f, b: %7.4f\n", _n, a, b);
      // (fabs(a) > 0.00001) ?  printf ("%f\n", a) : (fabs(b) > 0.00001) ? printf ("%f,-90\n", b) : printf ("0\n");
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
      // printf ("%f,%f\n", amp, phase);
      printf ("%f,%f\n", _n, a);
     
   }

   //Another way to get coefficients
   //Odd square should have odd harmonics (sin) 4/(pi*k) k=1,3,5,7,.... 
   /*
   float harms[] = {0, 4/M_PI, 0, 4/M_PI/3, 0, 4/M_PI/5, 0, 4/M_PI/7};
   for (int k = 1; k < 10; k += 2) 
   {
      printf ("%d: %f\n", k, 4/(M_PI*k));
   }
   */

}
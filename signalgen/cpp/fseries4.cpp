#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include "SignalGen.h"

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

float square_odd (float x) 
{
   float m =(float) fmod(x, M_PI*2);
   if (m < 0) 
     m = (float)M_PI*2 + m;
   return m < M_PI ? 1.0f : -1.0f;
}

float circle (float x) 
{
   float m = (float)fmod(x+M_PI/2, M_PI*2);
   if (m < 0) 
     m += (float)M_PI*2.0f;

   if (m > M_PI*2) 
   {
     m -= (float)M_PI*2.0f;
   }
    
   float theta;

   if (m <= M_PI)
   {
      theta = (float)acos ((m-M_PI/2)/M_PI*2);
   }
   else
   {
      theta = (float)-acos ((-M_PI+m-M_PI/2)/M_PI*2);
   }  
      
   return (float)sin(theta);

}


double stcos(double t, double f, double n, double(*st)(double))
{
   return st(t) * cos (2.0f * M_PI * f * n * t);
}

double stsin(double t, double f, double n, double(*st)(double))
{
   return st(t) * cos (2.0f * M_PI * f * n * t);
}

double simps3 (double a, double b, double(*f)(double)) 
{
    return f(a) + 4*f((a + b) / 2) + f(b);
}


double calc_simps (double a, double b, double(*f)(double)) 
{
    double dx = 0.01;

    int nsteps = (int)ceil (fabs(a - b) / dx); 

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


double calc_midpoint_rule(double a, double b, double n, double(*f)(double) )
{
  double dx = (b - a) / n;
  double sum = 0;
  for (int i = 0; i < n; ++i)
  {
    double p1 = dx * (double)i;
    double p2 = p1 + dx;
    double x = a + (p1 + p2) / 2.;
    sum += f(x) * dx;
  }

  return sum;
}


int main(void) 
{

    std::vector<double> harms; 
    std::vector<double> phases; 

   _T = 2*M_PI;
   _fptr = square;

   double nstep = 1;
   for (_n = 0; _n < 20; _n += nstep) 
   {
      // double a = 2/_T * calc_simps (-_T/2, _T/2, afunc);
      // double b = 2/_T * calc_simps (-_T/2, _T/2, bfunc);
      double a = 2/_T * calc_midpoint_rule (-_T/2, _T/2, 100000, afunc);
      double b = 2/_T * calc_midpoint_rule (-_T/2, _T/2, 100000, bfunc);
     
      
      double amp = sqrt(a*a + b*b);
      double phase = atan2(b, a); //*180/M_PI;
      if (amp < 0.00001) 
      {
         amp = 0; 
         phase = 0;
      }
      if (fabs(fabs(phase) - 180) < 0.01) 
      {
         phase = 0; 
         amp = -amp;
      }
      //printf ("%f,%f\n", amp, phase);
      //printf ("%f,%f\n", _n, 10*log(amp));
      //printf ("%f,%f\n", _n, a);
      harms.push_back(amp); 
      phases.push_back(phase);
     
   }

   //exit(1);

   int sps = 1000;
   int freq = 10;
   float sec = 1;
   float amp = 10;

   SignalGen signalGen(sps); 
   if (!signalGen.Generate(freq, harms, phases, sec)) 
   {
      exit(1);
   }

   double* res;
   int size = 0;
   signalGen.Get(size, res);
   for (int i = 0; i < size; ++i)
   {
       printf("%d, %f\n", i, res[i]);
   }

}
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

double square (double x) 
{
   return fabs (fmod (fabs(x), M_PI*2) < M_PI ? 1 : 0) - 0.5;
}

double square2 (double x) 
{
   static const double scale = M_PI/10;
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

    int nsteps = ceil (fabs(a - b) / dx); 

    double p = a;
    double result3 = 0; 
    for (int i = 0; i < nsteps; ++i)
    {
       result3 += simps3 (p, p+dx, f);
       p += dx; 
    }

    return dx/6 * result3;
}

double _T;
double(*_fptr)(double);
double _n;

double afunc (double x) 
{
  return _fptr(x)*cos(2*M_PI*1/_T*_n*x);
}

double bfunc (double x) 
{
  return _fptr(x)*sin(2*M_PI*1/_T*_n*x);
}

int main(void) 
{
   _T = 2*M_PI;
   _fptr = square;
   double nstep = .1;
   for (_n = -20; _n < 20; _n += nstep) 
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
      // printf ("%f,%f\n", amp, phase);
       printf ("%f,%f\n", _n, a);
     
   }


}
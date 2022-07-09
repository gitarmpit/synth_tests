#include <math.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <complex.h>

// complex simpson

double square (double x) 
{
   return fabs (fmod (fabs(x), M_PI*2) < M_PI ? 1 : 0) - 0.5;
}


double square2 (double x) 
{
    return fabs(x) < (M_PI-0.000001) ? 1 : 0;
}

double square_odd (double x) 
{
    return (x < 0) ? 0 : 1;
}


double square_odd2 (double x) 
{
    return fabs(x) < (M_PI-0.000001) ? (x > 0? 1: -1) : 0;
}


double test (double x) 
{
    return cos(2*M_PI*11*x) + cos(2*M_PI*3*x) + cos(2*M_PI*5*x);
}


double _n;  // frequency

double(*_fptr)(double);  // ptr to shape function (square etc)


double complex ft(double t) 
{
    //return cexp(-I * M_PI * 2 * _n * t) * _fptr(t); 
    return cexp(-I * _n * t) * _fptr(t); 
}

double complex simps3 (double a, double b) 
{
    return ft(a) + 4*ft((a + b) / 2) + ft(b);
}


double complex calc_simps (double a, double b) 
{
    double dx = M_PI/1000;

    int nsteps = ceil (fabs(a - b) / dx) + 1; 

    double p = a;
    double complex result3 = 0; 
    for (int i = 0; i < nsteps; ++i)
    {
       result3 += simps3 (p, p+dx);
       p += dx;
    }
    return dx/6 * result3;
}


int main(int argc, char**argv) 
{
    const double a = -M_PI;
    const double b = M_PI;
    double norm = M_PI;

    for (double k = -2*M_PI; k <= 2*M_PI; k += M_PI/100) 
    {
       //printf ("%f,%f\n", k, square2(k));
    }

    //exit(0);

   _fptr = square2;
   //const double nstep = .05;
   const double nstep = 1;
   const int nfreq = 20;
   const int cnt = (int)(nfreq * 2 / nstep);
   double complex ares [ cnt ];
   double nary [ cnt ];
   int i = 0;
   for (double n = -nfreq; n < nfreq; n += nstep) 
   {
      _n = n * 2 * M_PI; 
      double complex res = calc_simps (a, b) / norm;
      //printf("%f,%f\n", n, 10*log10(cabs(res)));
      ares[i] = res;
      nary[i++] = n;
      //printf ("%f: a: %f, b: %f, amp: %f, phase: %f\n", _n, creal(res), cimag(res), cabs(res), carg(res)*180/M_PI);
   }

   for (i = 0; i < cnt; ++i) 
   {
        //printf("%f,%f\n", nary[i], cabs(ares[i]));
   }

   //exit(1);


   const double a2 = 0;
   const double b2  = 2*M_PI;
   const double dx = 0.02;
   const int nsteps = ceil (fabs(b2 - a2) / dx) + 1;
   double tary[nsteps];

   double t = a2;
   for (int i = 0; i < nsteps; ++i) 
   {
       tary[i] = 0; 
       int j = 0;
       for (double n = -nfreq; n < nfreq; n += nstep) 
       {
            double complex res = ares[j];
            tary[i] += cabs(res) * cos(t*n*2*M_PI + carg(res)) * nstep;
            //tary[i] += creal(res) * cos(t*n*2*M_PI) * nstep;
            ++j;          
       }
       printf ("%f,%f\n", t, tary[i]);
       t += dx;
   }


}

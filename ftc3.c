#include <math.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <complex.h>


double rect (double x) 
{
    return fabs(x) < (M_PI-0.000001) ? 1 : 0;
}

double square_odd (double x) 
{
    return (x < 0) ? -0.5 : 0.5;
}


double _n;  // frequency

double(*_fptr)(double);  // ptr to shape function (square etc)


double complex ft(double t) 
{
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

   _fptr = square_odd;

   const double nstep = .1;
   const int nfreq = 5;
   for (double n = -nfreq; n <= nfreq; n += nstep) 
   {
      _n = n; // * 2 * M_PI; 
      double complex res = calc_simps (a, b) / norm;
      //printf("%f, a=%f b=%f, ph=%f \n", n, creal(res), cimag(res), carg(res)*180/M_PI);
      printf("%f, %f\n", n, cabs(res));
   }


}

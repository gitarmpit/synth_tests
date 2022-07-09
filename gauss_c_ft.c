#include <math.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <complex.h>

// Complex gauss


double _n;  // frequency
double(*_fptr)(double);  // ptr to shape function (square etc)

double complex f(double t) 
{
    return cexp(-I * M_PI * 2 * _n * t) * _fptr(t); 
}


double x3 [] = { 0.0000000000000000, -0.7745966692414834, 0.7745966692414834 };
double w3 [] = { 0.8888888888888888, 0.5555555555555556, 0.5555555555555556 };

double gauss3 () 
{
    double result = 0;
    for (int i = 0; i < 3; ++i)
    {
       result += w3[i] * f(x3[i]);
    }

    return result;
}

double x5 [] = { 0.0000000000000000, -0.5384693101056831, 0.5384693101056831, -0.9061798459386640, 0.9061798459386640 };
double w5 [] = { 0.5688888888888889, 0.4786286704993665, 0.4786286704993665, 0.2369268850561891, 0.2369268850561891 }; 

double w10 [] = { 
    0.2955242247147529,
    0.2955242247147529,
    0.2692667193099963,
    0.2692667193099963,
    0.2190863625159820,
    0.2190863625159820,
    0.1494513491505806,
    0.1494513491505806,
    0.0666713443086881,
    0.0666713443086881
};

double x10 [] = {
    -0.1488743389816312,
    0.1488743389816312,
    -0.4333953941292472,
    0.4333953941292472,
    -0.6794095682990244,
    0.6794095682990244,
    -0.8650633666889845,
    0.8650633666889845,
    -0.9739065285171717,
    0.9739065285171717
};

double complex gauss5 (double a, double b) 
{
    double complex result = 0;
    for (int i = 0; i < 5; ++i)
    {
       result += w5[i] * f((b-a)/2*x5[i] + (b + a)/2);
    }

    return (b-a)/2*result;
}

double complex gauss10 (double a, double b) 
{
    double complex result = 0;
    for (int i = 0; i < 10; ++i)
    {
       result += w10[i] * f((b-a)/2*x10[i] + (b + a)/2);
    }

    return (b-a)/2*result;
}

double square (double x) 
{
   return fabs (fmod (fabs(x), M_PI*2) < M_PI ? 1 : 0) - 0.5;
}


int main(int argc, char**argv) 
{
    
   _fptr = square;
   double nstep = .1;
   double a = -M_PI;
   double b = M_PI;
   for (_n = -16; _n < 16; _n += nstep) 
   {
        double complex res = gauss10(a, b);
        //printf("%.10f%+.10fi\n", creal(res), cimag(res));
        printf ("%f,%f\n", _n, creal(res));
   }



}

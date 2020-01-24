#include <math.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <complex.h>

double f(double x) 
{
    return x*x*x/14 -x*x/2 +x/2+0.5;
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
double gauss5 (double a, double b) 
{
    double result = 0;
    for (int i = 0; i < 5; ++i)
    {
       result += w5[i] * f((b-a)/2*x5[i] + (b + a)/2);
    }

    return (b-a)/2*result;
}



int main(int argc, char**argv) 
{
    double a = atof(argv[1]);
    double b = atof(argv[2]);
    double dx = atof(argv[3]);

    printf ("%f\n", gauss5(a, b));
    exit(0);

    int nsteps = ceil (fabs(a - b) / dx); 

    double p = a;
    for (int i = 0; i < nsteps; ++i)
    {
       p += dx; 
    }



}

#include <math.h> 
#include <stdio.h> 
#include <stdlib.h>

double f(double x) 
{
  //return x*x*x*sin(x);    
  //return exp(x);
  //return 1/x;
  return exp(-x)*cos(2*x);
  //return x*x;
}

double simps3 (double a, double b) 
{
    return f(a) + 4*f((a + b) / 2) + f(b);
}

double simps8 (double a, double b) 
{
    return f(a) + 3*f((2*a + b)/3) + 3*f((a + 2*b)/3)  + f(b);
}


int main(int argc, char**argv) 
{
    double a = atof(argv[1]);
    double b = atof(argv[2]);
    double dx = atof(argv[3]);

    // printf ("%f\n", tgamma(a));

    int nsteps = ceil (fabs(a - b) / dx); 

    double p = a;
    double result3 = 0; 
    double result8 = 0; 
    for (int i = 0; i < nsteps; ++i)
    {
       result3 += simps3 (p, p+dx);
       result8 += simps8 (p, p+dx);
       p += dx; 
    }


    printf ("%f\n", dx/6 * result3);
    printf ("%f\n", dx/8 * result8);

}


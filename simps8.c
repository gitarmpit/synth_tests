#include <math.h> 
#include <stdio.h> 
#include <stdlib.h>

double f(double x) 
{
  // return x*x;    
  return exp(x);
}

double simps (double a, double b) 
{
    return f(a) + 4*f((a + b) / 2) + f(b);
}

int main(int argc, char**argv) 
{
    double a = atof(argv[1]);
    double b = atof(argv[2]);
    double dx = atof(argv[3]);

    // printf ("%f\n", tgamma(a));

    int nsteps = ceil (fabs(a - b) / dx); 

    double p = a;
    double result = 0; 
    for (int i = 0; i < nsteps; ++i)
    {
       result += simps (p, p+dx);
       p += dx; 
    }


    printf ("%f\n", dx/3 * result / 2);

}

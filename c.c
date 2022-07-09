#include <math.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <complex.h>

double complex f(double x) 
{
    return cexp(I * x); 
}

double complex simps3 (double a, double b) 
{
    return f(a) + 4*f((a + b) / 2) + f(b);
}


int main(int argc, char**argv) 
{
    double a = atof(argv[1]);
    double b = atof(argv[2]);
    double dx = atof(argv[3]);

    int nsteps = ceil (fabs(a - b) / dx); 

    double p = a;
    double complex res = 0; 
    for (int i = 0; i < nsteps; ++i)
    {
       res += simps3 (p, p+dx);
       p += dx; 
    }

    //res * dx / 6

    printf ("res: %f\n", cabs(res) * dx / 6);

}

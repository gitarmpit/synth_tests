#include <math.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <complex.h>


int main(int argc, char**argv) 
{
    double complex z = 0;

    double a = 0; 
    double b = M_PI/4; 
    int nsteps = 1000; 
    double dx = fabs(a - b)/nsteps;
    double step = 2*M_PI/200;

    //for (int i = 0; i <= nsteps; ++i)
    int i = 0;
    for (double t = 0; t <= 2*M_PI; t += step )
    {
       //double complex z_tmp = cos(i*dx) + I*sin(i*dx);

       double complex z_tmp = cos(t) + I*sin(t);

       //printf ("arg: %f, abs:%f: %f + i%f\n", t, cabs(z_tmp), creal(z_tmp), cimag(z_tmp));
       printf ("%f,%f\n", creal(z_tmp), cimag(z_tmp));
       z += z_tmp;
       printf ("%f,%f\n", creal(z), cimag(z));
       ++i;
    }

    // printf ("n=%d, res: %f, %f + i%f\n", i, cabs(z)*step, creal(z), cimag(z));

}

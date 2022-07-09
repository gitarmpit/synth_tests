#include <stdio.h> 
#include <complex.h>
#include <math.h>

int main() 
{
    double PI = acos(-1);
    double complex z = cexp(I * PI); // Euler's formula
    printf("exp(i*pi) = %.1f%+.1fi\n", creal(z), cimag(z));
    double complex f[8] = { };
}

#include <math.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <complex.h>

double f2(double x) 
{
    return x*x*x/14 -x*x/2 +x/2+0.5;
}

double f(double x) 
{
    return 70+60*x+40*pow(x,2)-6*pow(x,3)+7*pow(x,4)-4*pow(x,5)-8*pow(x,6)-9*pow(x,7)-4*pow(x,8)+2.44*pow(x,9)+1.2*pow(x,10);
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

double gauss5 (double a, double b) 
{
    double result = 0;
    for (int i = 0; i < 5; ++i)
    {
       result += w5[i] * f((b-a)/2*x5[i] + (b + a)/2);
    }

    return (b-a)/2*result;
}

double gauss10 (double a, double b) 
{
    double result = 0;
    for (int i = 0; i < 10; ++i)
    {
       result += w10[i] * f((b-a)/2*x10[i] + (b + a)/2);
    }

    return (b-a)/2*result;
}


int main(int argc, char**argv) 
{
    double a = atof(argv[1]);
    double b = atof(argv[2]);

    printf ("%f\n", gauss10(a, b));
    exit(0);

    double dx = 1;
    int nsteps = ceil (fabs(a - b) / dx); 

    double p = a;
    for (int i = 0; i < nsteps; ++i)
    {
       p += dx; 
    }



}

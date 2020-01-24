#include <math.h> 
#include <stdio.h> 

double d (double x) 
{
  return 2*x;   
}

int main(void) 
{
  double step = 10;  
  double y = 0;
  double x = 0;
  do
  {
     double m0 = d(x);
     double m = d(x + step);
     y += step * (m0 + m) / 2;
     x += step;
     printf ("x: %f y: %f, err: %f\n", x, y, (x*x - y));
  }
  while (x <= 1000000);
}


#include "structs.h"

void int_switch(int* x, int* y)
{
  int t = *x;
  *x = *y;
  *y = t;
}

double frac(double x)
{
   return x - (int)x;
}

double invfrac(double x)
{
   return 1 - frac(x);
}

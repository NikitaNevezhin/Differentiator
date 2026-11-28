#ifndef FLOAT_TOOLS_H

#define FLOAT_TOOLS_H

#include <math.h>
#include <stdbool.h>

#define ACCURACY 10e-6

bool    IsFloatEqual   (const double x1, const double x2);

bool    IsGreater      (const double x1, const double x2);

bool    IsInt          (const double x);

bool    IsNatural      (const double x);


#endif
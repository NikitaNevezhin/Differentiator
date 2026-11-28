#ifndef FLOAT_TOOLS_CPP

#define FLOAT_TOOLS_CPP

#include "FloatTools.h"

bool IsFloatEqual(const double x, const double y)
{
    return fabs(x - y) < ACCURACY;
}

bool IsGreater(const double x1, const double x2)
{
    return (x1 - x2) > ACCURACY;
}

bool IsInt(const double x)
{
    return IsFloatEqual(x, trunc(x));
}

bool IsNatural(const double x)
{
    return IsInt(x) && x > 0;
}

#endif
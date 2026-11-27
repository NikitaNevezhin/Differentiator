#include "op_calc.h"

#include <math.h>
#include "../utils/PrintColor.h"
#include "../utils/FloatTools.h"

double CalcAdd(double a, double b)
{
    return a + b;
}

double CalcSub(double a, double b)
{
    return a - b;
}

double CalcMul(double a, double b)
{
    return a * b;
}

double CalcDiv(double a, double b)
{
    if (IsFloatEqual(b, 0))
    {
        PRINT_RED("Zeror division error in %s\n", __func__);
        return NAN;
    }

    return a / b;
}

double CalcPow(double a, double b)
{
    return pow(a, b);
}

double CalcSin(double a, double b)
{
    (void)b; // escaping unused variable error   
    return sin(a);
}

double CalcCos(double a, double b)
{
    (void)b; // escaping unused variable error
    return cos(a);
}

double CalcTan(double a, double b)
{
    (void)b; // escaping unused variable error
    return tan(a);
}

double CalcCot(double a, double b)
{
    (void)b;
    return 1 / tan(a);
}

double CalcLn(double a, double b)
{
    (void)b;
    return log(a);
}
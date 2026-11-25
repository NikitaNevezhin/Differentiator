#include "op_table.h"

// -----------------------------
//        CALC FUNCTIONS
// -----------------------------

static double CalcAdd   (double a, double b);

static double CalcSub   (double a, double b);

static double CalcMul   (double a, double b);

static double CalcDiv   (double a, double b);

static double CalcPow   (double a, double b);

static double CalcSin   (double a, double b);


static double CalcAdd(double a, double b)
{
    return a + b;
}

static double CalcSub(double a, double b)
{
    return a - b;
}

static double CalcMul(double a, double b)
{
    return a * b;
}

static double CalcDiv(double a, double b)
{
    if (IsFloatEqual(b, 0))
    {
        PRINT_RED("Zeror division error in %s\n", __func__);
        return NAN;
    }

    return a / b;
}

static double CalcPow(double a, double b)
{
    return pow(a, b);
}

static double CalcSin(double a, double b)
{
    (void)b; // escaping unused variable error   
    return sin(a);
}

const OpInfo OP_TABLE[TOTAL_OPS] =
{
    {ADD,   "+", "ADD", 2, CalcAdd},              // USER!!!!!!!!!!!
    {SUB,   "-", "SUB", 2, CalcSub},              // DONT CHANGE THE ORDER!!!!!
    {MUL,   "*", "MUL", 2, CalcMul},              // GET THE FUCK OUT!!!!!!!!!!
    {DIV,   "/", "DIV", 2, CalcDiv},              // please
    {POW,   "^", "POW", 2, CalcPow},
    {SIN, "sin", "SIN", 1, CalcSin}
};
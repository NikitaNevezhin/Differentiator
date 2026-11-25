#include "op_table.h"

#include "../utils/FloatTools.h"

#include "../diff/diff.h"
#include "../diff/dsl.h"

// -----------------------------
//        CALC FUNCTIONS
// -----------------------------

static double   CalcAdd   (double a, double b);

static double   CalcSub   (double a, double b);

static double   CalcMul   (double a, double b);

static double   CalcDiv   (double a, double b);

static double   CalcPow   (double a, double b);

static double   CalcSin   (double a, double b);

// -----------------------------
//        DIFF FUNCTIONS
// -----------------------------

static Node* DiffAdd    (const Node* node, VarIdx diff_var);

static Node* DiffSub    (const Node* node, VarIdx diff_var);

static Node* DiffMul    (const Node* node, VarIdx diff_var);

static Node* DiffDiv    (const Node* node, VarIdx diff_var);

static Node* DiffPow    (const Node* node, VarIdx diff_var);

static Node* DiffSin    (const Node* node, VarIdx diff_var);


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


static Node* DiffAdd(const Node* node, VarIdx diff_var)
{
    return _ADD(_DL, _DR);
}

static Node* DiffSub(const Node* node, VarIdx diff_var)
{
    return _SUB(_DL, _DR);
}

static Node* DiffMul(const Node* node, VarIdx diff_var)
{
    return _ADD(_MUL(_DL, _CR), _MUL(_CL, _DR));
}

static Node* DiffDiv(const Node* node, VarIdx diff_var)
{
    return _DIV(_SUB(_MUL(_DL, _CR), _MUL(_CL, _DR)), _MUL(_CR, _CR));
}

static Node* DiffPow(const Node* node, VarIdx diff_var)
{
    if (ContainsDiffVar(node->right, diff_var))
    {
        PRINT_RED("Exponent functions diff is still in process!\n");
        return NULL;
    }

    if (ContainsDiffVar(node->left, diff_var))
    {
        // diff(f^c) = diff(f) * f ^ (c - 1)
        return _MUL(_DL, _MUL(_CR, _POW(_CL, _SUB(_CR, _NUM(1)))));
    }

    return _NUM(0); // const ^ const
}

const OpInfo OP_TABLE[TOTAL_OPS] =
{
    {ADD,   "+", "ADD", 2, CalcAdd, DiffAdd},              // USER!!!!!!!!!!!
    {SUB,   "-", "SUB", 2, CalcSub, DiffSub},              // DONT CHANGE THE ORDER!!!!!
    {MUL,   "*", "MUL", 2, CalcMul, DiffMul},              // GET THE FUCK OUT!!!!!!!!!!
    {DIV,   "/", "DIV", 2, CalcDiv, DiffDiv},              // please
    {POW,   "^", "POW", 2, CalcPow, DiffPow},
    {SIN, "sin", "SIN", 1, CalcSin, NULL}
};
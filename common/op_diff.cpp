#include "op_diff.h"

#include "../diff/diff.h"
#include "../diff/dsl.h"

Node* DiffAdd(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _ADD(_DL, _DR);
}

Node* DiffSub(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _SUB(_DL, _DR);
}

Node* DiffMul(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _ADD(_MUL(_DL, _CR), _MUL(_CL, _DR));
}

Node* DiffDiv(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _DIV(_SUB(_MUL(_DL, _CR), _MUL(_CL, _DR)), _MUL(_CR, _CR));
}

Node* DiffPow(const Node* node, VarIdx diff_var)
{
    assert(node);
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

Node* DiffSin(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _MUL(_COS(_CL), _DL);  // (sin(f))' = cos(f) * f'
}

Node* DiffCos(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _NEG(_MUL(_SIN(_CL), _DL));  // (cos(f))' = -sin(f) * f'
}


Node* DiffTan(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _MUL(_DL, _DIV(_NUM(1), _POW(_COS(_CL), _NUM(2)))); // (tan(f))' = 1/(cos(f)^2) * f'
}

Node* DiffCot(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _NEG(_MUL(_DL, _DIV(_NUM(1), _POW(_SIN(_CL), _NUM(2))))); // (cot(f))' = -1 / (sin(f)^2) * f'
}

Node* DiffLn(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _MUL(_DL, _DIV(_NUM(1), _CL)); // (ln(f))' = (1 / f) * f'
}
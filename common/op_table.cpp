#include "op_table.h"

#include <assert.h>

#include "../utils/FloatTools.h"
#include "../utils/PrintColor.h"

#include "../diff/diff.h"
#include "../diff/dsl.h"

#include "../diff/tex_dump.h"

// -----------------------------
//        CALC FUNCTIONS
// -----------------------------

static double   CalcAdd   (double a, double b);

static double   CalcSub   (double a, double b);

static double   CalcMul   (double a, double b);

static double   CalcDiv   (double a, double b);

static double   CalcPow   (double a, double b);

static double   CalcSin   (double a, double b);

static double   CalcCos   (double a, double b);

static double   CalcTan   (double a, double b);

// -----------------------------
//        DIFF FUNCTIONS
// -----------------------------

static Node*    DiffAdd    (const Node* node, VarIdx diff_var);

static Node*    DiffSub    (const Node* node, VarIdx diff_var);

static Node*    DiffMul    (const Node* node, VarIdx diff_var);

static Node*    DiffDiv    (const Node* node, VarIdx diff_var);

static Node*    DiffPow    (const Node* node, VarIdx diff_var);

static Node*    DiffSin    (const Node* node, VarIdx diff_var);

static Node*    DiffCos    (const Node* node, VarIdx diff_var);

static Node*    DiffTan    (const Node* node, VarIdx diff_var);

// -----------------------------
//      TEX_DUMP FUNCTIONS
// -----------------------------

static int   TexDumpAdd     (FILE* tex_file, Node* node);

static int   TexDumpSub     (FILE* tex_file, Node* node);

static int   TexDumpMul     (FILE* tex_file, Node* node);

static int   TexDumpDiv     (FILE* tex_file, Node* node);

static int   TexDumpPow     (FILE* tex_file, Node* node);

static int   TexDumpSin     (FILE* tex_file, Node* node);

static int   TexDumpCos     (FILE* tex_file, Node* node);

static int   TexDumpTan     (FILE* tex_file, Node* node);


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

static double CalcCos(double a, double b)
{
    (void)b; // escaping unused variable error
    return cos(a);
}

static double CalcTan(double a, double b)
{
    (void)b; // escaping unused variable error
    return tan(a);
}


static Node* DiffAdd(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _ADD(_DL, _DR);
}

static Node* DiffSub(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _SUB(_DL, _DR);
}

static Node* DiffMul(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _ADD(_MUL(_DL, _CR), _MUL(_CL, _DR));
}

static Node* DiffDiv(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _DIV(_SUB(_MUL(_DL, _CR), _MUL(_CL, _DR)), _MUL(_CR, _CR));
}

static Node* DiffPow(const Node* node, VarIdx diff_var)
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

static Node* DiffSin(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _MUL(_COS(_CL), _DL);  // (sin(f))' = cos(f) * f'
}

static Node* DiffCos(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _NEG(_MUL(_SIN(_CL), _DL));  // (cos(f))' = -sin(f) * f'
}


static Node* DiffTan(const Node* node, VarIdx diff_var)
{
    assert(node);
    return _MUL(_DL, _DIV(_NUM(1), _POW(_COS(_CL), _NUM(2)))); // (tan(f))' = 1/(cos(f)^2) * f'
}

static int TexDumpAdd(FILE* tex_file, Node* node)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\left(");
    TexDumpNode(tex_file, node->left);
    fprintf(tex_file, " + ");
    TexDumpNode(tex_file, node->right);
    fprintf(tex_file, "\\right)");

    return 0;
}

static int TexDumpSub(FILE* tex_file, Node* node)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\left(");
    if (!_LEFT_0) TexDumpNode(tex_file, node->left);
    fprintf(tex_file, " - ");
    TexDumpNode(tex_file, node->right);
    fprintf(tex_file, "\\right)");

    return 0;
}

static int TexDumpMul(FILE* tex_file, Node* node)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\left(");
    TexDumpNode(tex_file, node->left);
    fprintf(tex_file, " \\cdot ");
    TexDumpNode(tex_file, node->right);
    fprintf(tex_file, "\\right)");

    return 0;
}

static int TexDumpDiv(FILE* tex_file, Node* node)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\left(");
    fprintf(tex_file, "\\frac{");
    TexDumpNode(tex_file, node->left);
    fprintf(tex_file, "}");
    fprintf(tex_file, "{");
    TexDumpNode(tex_file, node->right);
    fprintf(tex_file, "}");
    fprintf(tex_file, "\\right)");

    return 0;
}

static int TexDumpPow(FILE* tex_file, Node* node)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\left(");
    TexDumpNode(tex_file, node->left);
    fprintf(tex_file, "\\right)");
    fprintf(tex_file, "^");
    fprintf(tex_file, "{");
    TexDumpNode(tex_file, node->right);
    fprintf(tex_file, "}");

    return 0;
}

static int TexDumpSin(FILE* tex_file, Node* node)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\sin");
    fprintf(tex_file, "(");
    TexDumpNode(tex_file, node->left);
    fprintf(tex_file, ")");

    return 0;
}

static int TexDumpCos(FILE* tex_file, Node* node)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\cos");
    fprintf(tex_file, "(");
    TexDumpNode(tex_file, node->left);
    fprintf(tex_file, ")");

    return 0;
}

static int TexDumpTan(FILE* tex_file, Node* node)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\tan");
    fprintf(tex_file, "(");
    TexDumpNode(tex_file, node->left);
    fprintf(tex_file, ")");

    return 0;
}

const OpInfo OP_TABLE[TOTAL_OPS] =
{
    {ADD,   "+", "ADD", 2,      CalcAdd, DiffAdd, TexDumpAdd},              // USER!!!!!!!!!!!
    {SUB,   "-", "SUB", 2,      CalcSub, DiffSub, TexDumpSub},              // DONT CHANGE THE ORDER!!!!!
    {MUL,   "*", "MUL", 2,      CalcMul, DiffMul, TexDumpMul},              // GET THE FUCK OUT!!!!!!!!!!
    {DIV,   "/", "DIV", 2,      CalcDiv, DiffDiv, TexDumpDiv},              // please
    {POW,   "^", "POW", 2,      CalcPow, DiffPow, TexDumpPow},
    {SIN, "sin", "SIN", 1,      CalcSin, DiffSin, TexDumpSin},
    {COS, "cos", "COS", 1,      CalcCos, DiffCos, TexDumpCos},
    {TAN, "tan", "TAN", 1,      CalcTan, DiffTan, TexDumpTan}
};
#include "op_tex.h"

#include "../diff/dsl.h"
#include "../diff/tex_dump.h"

int TexDumpAdd(FILE* tex_file, Node* node)
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

int TexDumpSub(FILE* tex_file, Node* node)
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

int TexDumpMul(FILE* tex_file, Node* node)
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

int TexDumpDiv(FILE* tex_file, Node* node)
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

int TexDumpPow(FILE* tex_file, Node* node)
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

int TexDumpSin(FILE* tex_file, Node* node)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\sin");
    fprintf(tex_file, "(");
    TexDumpNode(tex_file, node->left);
    fprintf(tex_file, ")");

    return 0;
}

int TexDumpCos(FILE* tex_file, Node* node)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\cos");
    fprintf(tex_file, "(");
    TexDumpNode(tex_file, node->left);
    fprintf(tex_file, ")");

    return 0;
}

int TexDumpTan(FILE* tex_file, Node* node)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\tan");
    fprintf(tex_file, "(");
    TexDumpNode(tex_file, node->left);
    fprintf(tex_file, ")");

    return 0;
}

int TexDumpCot(FILE* tex_file, Node* node)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\cot");
    fprintf(tex_file, "(");
    TexDumpNode(tex_file, node->left);
    fprintf(tex_file, ")");

    return 0;
}

int TexDumpLn(FILE* tex_file, Node* node)
{
    assert(tex_file);
    assert(node);

    fprintf(tex_file, "\\ln");
    fprintf(tex_file, "(");
    TexDumpNode(tex_file, node->left);
    fprintf(tex_file, ")");

    return 0;
}
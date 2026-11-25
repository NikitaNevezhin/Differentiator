#ifndef OP_TABLE_H

#define OP_TABLE_H

#include "../tree/tree.h"

typedef double (*CalcFunc)(double, double);

typedef Node*  (*DiffFunc)(const Node*, VarIdx);

struct OpInfo
{
    OpType op;
    const char* designation;
    const char* op_name;
    int arg_amount;

    CalcFunc calc_func;
    DiffFunc diff_func;
};

extern const OpInfo OP_TABLE[TOTAL_OPS];

#endif
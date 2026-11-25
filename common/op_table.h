#ifndef OP_TABLE_H

#define OP_TABLE_H

#include "../tree/tree.h"

struct OpInfo
{
    OpType op;
    const char* designation;
    const char* op_name;
    int arg_amount;
};

extern const OpInfo OP_TABLE[TOTAL_OPS];

#endif
#include "calc.h"

#include "../common/op_table.h"

// --------------------------------
//        HELPER FUNCTIONS
// --------------------------------

static double       CalcOp              (OpType op_type, double a, double b);


static double CalcOp(OpType op_type, double a, double b)
{
    return OP_TABLE[op_type].calc_func(a, b);
}

double CalcTree(Node* root, const double* var_values)
{
    assert(root);

    switch (root->node_type)
    {
        case NUM:
        {
            return root->val.num_val;
        }

        case VAR:
        {
            return var_values[root->val.var_idx];
        }

        case OP:
        {
            return CalcOp(root->val.op_type, 
                           CalcTree(root->left, var_values), CalcTree(root->right, var_values));
        }

        default: break;
    }

    return NAN;
}
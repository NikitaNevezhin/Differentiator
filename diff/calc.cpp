#include "calc.h"

// --------------------------------
//        HELPER FUNCTIONS
// --------------------------------

static double       CalcOp              (OpType op_type, double a, double b);


static double CalcOp(OpType op_type, double a, double b)
{

    double res = NAN;

    switch (op_type)
    {
        case ADD: res = a + b;
                  break;

        case SUB: res = a - b;
                  break;

        case MUL: res = a * b;
                  break;

        case DIV:
        {
            if (IsFloatEqual(b, 0)) 
            {
                res = NAN;
                PRINT_RED("ZERO DIVISION ERROR\n");
            }

            else res = a / b;
            
            break;
        }

        case POW: res = pow(a, b);
                  break;

        default: res = NAN;
    }

    return res;
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

        default: return NAN;
    }

    return NAN;
}
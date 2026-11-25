#include "diff.h"

#include "dsl.h"

// --------------------------------
//        HELPER FUNCTIONS
// --------------------------------

static bool     ContainsDiffVar             (const Node* node, VarIdx diff_var);


static bool ContainsDiffVar(const Node* node, VarIdx diff_var)
{
    if (node == NULL) return false;

    if (node->node_type == VAR) return node->val.var_idx == diff_var;

    return ContainsDiffVar(node->left, diff_var) || ContainsDiffVar(node->right, diff_var);
}


Node* Diff(const Node* node, VarIdx diff_var)
{
    if (node == NULL) return NULL;

    switch (node->node_type)
    {
        case NUM: return _NUM(0);
        
        case VAR: 
        {   
            if (node->val.var_idx == diff_var) return _NUM(1);
            else return _NUM(0);
        }
        case OP:
        {
            switch (node->val.op_type)
            {
                case ADD: return _ADD(_DL, _DR);

                case SUB: return _SUB(_DL, _DR);

                case MUL: return _ADD(_MUL(_DL, _CR), _MUL(_CL, _DR));

                case DIV: return _DIV(_SUB(_MUL(_DL, _CR), _MUL(_CL, _DR)), _MUL(_CR, _CR));

                case POW:
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

                default: return NULL;
            }
        }

        default: return NULL;
    }

    return NULL;
}

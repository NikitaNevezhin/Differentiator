#include "diff.h"

#include "dsl.h"

#include "../common/op_table.h"

bool ContainsDiffVar(const Node* node, VarIdx diff_var)
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

        case OP: return OP_TABLE[node->val.op_type].diff_func(node, diff_var);

        default: break;
    }

    return NULL;
}

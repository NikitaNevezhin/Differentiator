#include "diff.h"

// -------------------------------------------
// DSL for convinient work with function Diff
// -------------------------------------------

#define     _NUM(val)           NewNode(NUM, {val, OP_NONE, VAR_NONE }, NULL, NULL)
#define     _VAR(val)           NewNode(VAR, {NAN, OP_NONE,      val }, NULL, NULL)

#define     _ADD(left, right)   NewNode(OP, {NAN, ADD, VAR_NONE}, left, right)
#define     _SUB(left, right)   NewNode(OP, {NAN, SUB, VAR_NONE}, left, right)
#define     _MUL(left, right)   NewNode(OP, {NAN, MUL, VAR_NONE}, left, right)
#define     _DIV(left, right)   NewNode(OP, {NAN, DIV, VAR_NONE}, left, right)

#define     _DL                 Diff(node->left, diff_var)
#define     _DR                 Diff(node->right, diff_var)

#define     _CL                 CopySubtree(node->left)
#define     _CR                 CopySubtree(node->right)


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

                default: return NULL;
            }
        }

        default: return NULL;
    }

    return NULL;
}
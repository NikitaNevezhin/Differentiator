#include "diff.h"

// -------------------------------------------
// DSL for convinient work with function Diff
// -------------------------------------------

#define     _NUM(val)           NewNode(NUM, {val, OP_NONE, -1 }, NULL, NULL)
#define     _VAR(val)           NewNode(VAR, {NAN, OP_NONE, val}, NULL, NULL)

#define     _ADD(left, right)   NewNode(OP, {NAN, ADD, -1}, left, right)
#define     _SUB(left, right)   NewNode(OP, {NAN, SUB, -1}, left, right)
#define     _MUL(left, right)   NewNode(OP, {NAN, MUL, -1}, left, right)
#define     _DIV(left, right)   NewNode(OP, {NAN, DIV, -1}, left, right)

#define     _DL                 Diff(node->left)
#define     _DR                 Diff(node->right)

#define     _CL                 CopySubtree(node->left)
#define     _CR                 CopySubtree(node->right)


Node* Diff(const Node* node)
{
    if (node == NULL) return NULL;

    switch (node->node_type)
    {
        case NUM: return _NUM(0);
        
        case VAR: return _NUM(1);

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
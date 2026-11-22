#include "diff.h"

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
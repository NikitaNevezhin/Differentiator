#ifndef DSL_H

#define DSL_H

#include <math.h>
#include "../tree/tree.h"

// -------------------------------------------
// DSL for convinient work with function Diff
// -------------------------------------------

#define     _NUM(val)           NewNode(NUM, (NodeVal){.num_val = val}, NULL, NULL)
#define     _VAR(val)           NewNode(VAR, (NodeVal){.var_idx = val}, NULL, NULL)

#define     _ADD(left, right)   NewNode(OP,  (NodeVal){.op_type = ADD},  left, right)
#define     _SUB(left, right)   NewNode(OP,  (NodeVal){.op_type = SUB},  left, right)
#define     _MUL(left, right)   NewNode(OP,  (NodeVal){.op_type = MUL},  left, right)
#define     _DIV(left, right)   NewNode(OP,  (NodeVal){.op_type = DIV},  left, right)
#define     _POW(left, right)   NewNode(OP,  (NodeVal){.op_type = POW},  left, right)

#define     _SIN(left)          NewNode(OP, (NodeVal){.op_type = SIN},  left, NULL)
#define     _COS(left)          NewNode(OP, (NodeVal){.op_type = COS},  left, NULL)

#define     _DL                 Diff(node->left, diff_var)
#define     _DR                 Diff(node->right, diff_var)

#define     _CL                 CopySubtree(node->left)
#define     _CR                 CopySubtree(node->right)

#define     _LEFT_0             (node->left->node_type == NUM && IsFloatEqual(node->left->val.num_val, 0))
#define     _LEFT_1             (node->left->node_type == NUM && IsFloatEqual(node->left->val.num_val, 1))
#define     _RIGHT_0            (node->right->node_type == NUM && IsFloatEqual(node->right->val.num_val, 0))
#define     _RIGHT_1            (node->right->node_type == NUM && IsFloatEqual(node->right->val.num_val, 1))

#define     _NEG(left)          _SUB(_NUM(0), CopySubtree(left))

#endif

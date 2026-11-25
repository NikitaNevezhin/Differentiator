#ifndef DSL_H

#define DSL_H

#include <math.h>
#include "../tree/tree.h"

// -------------------------------------------
// DSL for convinient work with function Diff
// -------------------------------------------

#define     _NUM(val)           NewNode(NUM, {val, OP_NONE, VAR_NONE }, NULL, NULL)
#define     _VAR(val)           NewNode(VAR, {NAN, OP_NONE,      val }, NULL, NULL)

#define     _ADD(left, right)   NewNode(OP,  {NAN, ADD, VAR_NONE},  left, right)
#define     _SUB(left, right)   NewNode(OP,  {NAN, SUB, VAR_NONE},  left, right)
#define     _MUL(left, right)   NewNode(OP,  {NAN, MUL, VAR_NONE},  left, right)
#define     _DIV(left, right)   NewNode(OP,  {NAN, DIV, VAR_NONE},  left, right)
#define     _POW(left, right)   NewNode(OP,  {NAN, POW, VAR_NONE},  left, right)

#define     _DL                 Diff(node->left, diff_var)
#define     _DR                 Diff(node->right, diff_var)

#define     _CL                 CopySubtree(node->left)
#define     _CR                 CopySubtree(node->right)

#define     _LEFT_0             (node->left->node_type == NUM && IsFloatEqual(node->left->val.num_val, 0))
#define     _LEFT_1             (node->left->node_type == NUM && IsFloatEqual(node->left->val.num_val, 1))
#define     _RIGHT_0            (node->right->node_type == NUM && IsFloatEqual(node->right->val.num_val, 0))
#define     _RIGHT_1            (node->right->node_type == NUM && IsFloatEqual(node->right->val.num_val, 1))

#endif
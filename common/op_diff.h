#ifndef OP_DIFF_H

#define OP_DIFF_H

#include "../tree/tree.h"

// -----------------------------
//        DIFF FUNCTIONS
// -----------------------------

Node*    DiffAdd        (const Node* node, VarIdx diff_var);
    
Node*    DiffSub        (const Node* node, VarIdx diff_var);
    
Node*    DiffMul        (const Node* node, VarIdx diff_var);
    
Node*    DiffDiv        (const Node* node, VarIdx diff_var);
    
Node*    DiffPow        (const Node* node, VarIdx diff_var);
    
Node*    DiffSin        (const Node* node, VarIdx diff_var);
    
Node*    DiffCos        (const Node* node, VarIdx diff_var);
    
Node*    DiffTan        (const Node* node, VarIdx diff_var);
    
Node*    DiffCot        (const Node* node, VarIdx diff_var);
    
Node*    DiffLn         (const Node* node, VarIdx diff_var);
    
Node*    DiffArcsin     (const Node* node, VarIdx diff_var);

Node*    DiffArccos     (const Node* node, VarIdx diff_var);

Node*    DiffArctan     (const Node* node, VarIdx diff_var);

#endif
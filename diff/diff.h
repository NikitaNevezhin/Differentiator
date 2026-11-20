#ifndef DIFF_H

#define DIFF_H

#include <ctype.h>
#include <string.h>
#include <math.h>
#include "../utils/FloatTools.h"

#include "../tree/tree.h"

// -------------------------------------------
// DSL for convinient work with function Diff
// -------------------------------------------

#define     _NUM(val)           NewNode(NUM, val, NULL, NULL)
#define     _VAR(val)           NewNode(VAR, val, NULL, NULL)

#define     _ADD(left, right)   NewNode(OP, (double)ADD, left, right)
#define     _SUB(left, right)   NewNode(OP, (double)SUB, left, right)
#define     _MUL(left, right)   NewNode(OP, (double)MUL, left, right)
#define     _DIV(left, right)   NewNode(OP, (double)DIV, left, right)

#define     _DL                 Diff(node->left)
#define     _DR                 Diff(node->right)

#define     _CL                 CopySubtree(node->left)
#define     _CR                 CopySubtree(node->right)

// -------------------------------------------
//          VALUES FOR VARIABLES
// -------------------------------------------

// -------------------------------------------
//         EXPRESSION PARSER SECTION
// -------------------------------------------

int          ParseData              (char* data_buff, FILE* tree_file);
    
int          GetNodeInfo            (const char* val_buff, NodeType* node_type, double* val);
    
Node*        ParsePrefixSubtree     (FILE* tree_file, int* total_nodes);
    
Node*        ParseInfixSubtree      (FILE* tree_file, int* total_nodes);
    
int          ParsePrefixTree        (Tree* tree, const char* tree_filename);
    
int          ParseInfixTree         (Tree* tree, const char* tree_filename);
    
void         PrintTree              (Node* root);
    
void         FprintTree             (FILE* tree_file, Node* root);

// -------------------------------------------
//          DIFFERENTIATOR SECTION
// -------------------------------------------

Node*        Diff                (const Node* node);

// -------------------------------------------
//           CALCULATOR SECTION
// -------------------------------------------

double       CalcOp              (OpType op_type, double a, double b);

double       CalcTree            (Node* root, const double* var_values);

#endif
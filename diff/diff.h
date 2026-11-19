#ifndef DIFF_H

#define DIFF_H

#include <ctype.h>
#include <string.h>
#include <math.h>
#include "../utils/FloatTools.h"

#include "../tree/tree.h"

// -------------------------------------------
// DSL for convinient work with function diff
// -------------------------------------------

#define _NUM(val)           NewNode(NUM, val, NULL, NULL)
#define _VAR(val)           NewNode(VAR, val, NULL, NULL)

#define _ADD(left, right)   NewNode(OP, (double)ADD, left, right)
#define _SUB(left, right)   NewNode(OP, (double)SUB, left, right)
#define _MUL(left, right)   NewNode(OP, (double)MUL, left, right)
#define _DIV(left, right)   NewNode(OP, (double)DIV, left, right)

// -------------------------------------------
//          VALUES FOR VARIABLES
// -------------------------------------------

const double VAR_VALUES[TOTAL_VARS]
{
    5,
    2,
    3
};


// -------------------------------------------
//         EXPRESSION PARSER SECTION
// -------------------------------------------

int          ParseData           (char* data_buff, FILE* tree_file);

int          GetNodeInfo         (const char* val_buff, NodeType* node_type, double* val);

Node*        ParseSubtree        (FILE* tree_file, int* total_nodes);

int          ParseTree           (Tree* tree, const char* tree_filename);

void         PrintTree           (Node* root);

void         FprintTree          (FILE* tree_file, Node* root);

// -------------------------------------------
//          DIFFERENTIATOR SECTION
// -------------------------------------------

//;


// -------------------------------------------
//            EVALUATOR SECTION
// -------------------------------------------

double       CalcOp              (OpType op_type, double a, double b);

double       CalcTree            (Node* root);

#endif
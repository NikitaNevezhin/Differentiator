#ifndef TREE_H

#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../utils/PrintColor.h"
#include "../utils/StringTools.h"

// ---------------------------------------------
//              General defines
// ---------------------------------------------

#define POIZON -1776.911

#define TERM_CMD_MAX_SIZE 256  // idk, let it be 256

#define TREE_GRAPH_DUMP(tree, html_filename) TreeGraphDump(tree, html_filename, __FILE__, __LINE__, __func__);

// ---------------------------------------------
// DSL for convinient work with function NewNode
// ---------------------------------------------

#define _NUM(val)           NewNode(NUM, val, NULL, NULL)
#define _VAR(val)           NewNode(VAR, val, NULL, NULL)

#define _ADD(left, right)   NewNode(OP, (double)ADD, left, right)
#define _SUB(left, right)   NewNode(OP, (double)SUB, left, right)
#define _MUL(left, right)   NewNode(OP, (double)MUL, left, right)
#define _DIV(left, right)   NewNode(OP, (double)DIV, left, right)



typedef double node_t;

enum NodeType
{
    OP,
    NUM,
    VAR
};

enum Operation
{
    ADD,
    SUB,
    MUL,
    DIV
};

struct Node
{
    NodeType node_type;
    double val;

    Node* left;
    Node* right;

    Node* parent;
};

struct Tree
{
    Node* root;
    int total_nodes;
};

// -----------------
//   GENERAL TOOLS
// -----------------

Node*        NewNode             (NodeType node_type, double val, Node* left, Node* right);
 
int          NodeDestroy         (Node* node);

int          TreeInit            (Tree* tree);

int          TreeDestroy         (Tree* tree);
 
int          LinkLeft            (Node* parent, Node* child);

int          LinkRight           (Node* parent, Node* child);

bool         IsLeaf              (Node* node);
 
void         PrintTree           (Node* root);
   
// ------------------
//  TREE-FILE PARSER
// ------------------

bool         IsOperator          (const char* s);

bool         IsVariable          (const char* s);

bool         IsNumber            (const char* val);

int          ParseData           (char* data_buff, FILE* tree_file);

int          GetNodeInfo         (const char* val_buff, NodeType* node_type, double* val);

Node*        ParseSubtree        (FILE* tree_file, int* total_nodes);

int          ParseTree           (Tree* tree, const char* tree_filename);

void         FprintTree          (FILE* tree_file, Node* root);

// ------------------
// GRAPH DUMP SECTION
// ------------------

const char*     GetStrNodeType          (NodeType node_type);

int             GenerateNodeDump        (FILE* dot_file, const Node* node, int node_idx, int dump_counter);
        
int             GenerateTreeDump        (FILE* dot_file, const Node* root, int* node_idx, int dump_counter);
        
int             GenerateDotFile         (Node* root, const char* dot_filename, int dump_counter);
        
int             TreeGenerateHtmlFile    (Tree* tree,  const char* html_filename, 
                                         const char* file, int line, const char* func);
        
int             TreeGraphDump           (Tree* tree, const char* html_filename, 
                                         const char* file, int line, const char* func);
                                 
#endif

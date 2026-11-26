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
#define VAL_MAX_SIZE 256       // still no idea what to choose

#define TOTAL_NODE_TYPES 3
#define TOTAL_OPS 7
#define TOTAL_VARS 3

#define TREE_GRAPH_DUMP(tree, html_filename) TreeGraphDump(tree, html_filename, __FILE__, __LINE__, __func__);

// ----------------------------------
//        GENERAL STRUCTURES
// ----------------------------------

enum NodeType
{
    NUM,
    VAR,
    OP
};

enum OpType
{
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    SIN,
    COS,
    OP_NONE
};

enum VarIdx
{
    VAR_X,
    VAR_Y,
    VAR_Z,
    VAR_NONE = -1
};

union NodeVal
{
    double num_val;
    OpType op_type;
    VarIdx var_idx;
};

struct Node
{
    NodeType node_type;
    NodeVal val;

    Node* left;
    Node* right;

    Node* parent;
};

struct Tree
{
    Node* root;
    int total_nodes;
};

struct VarInfo
{
    VarIdx idx;
    const char* name;
};

const VarInfo VAR_TABLE[TOTAL_VARS] =
{
    {VAR_X, "x"},
    {VAR_Y, "y"},
    {VAR_Z, "z"}
};

// -------------------------------------
//      HELPER STRUCTURES FOR DUMP
// -------------------------------------

struct NodeTypeInfo
{
    NodeType node_type;
    const char* type_name;
    const char* bgcolor;
};

const NodeTypeInfo NODE_TYPE_TABLE[TOTAL_NODE_TYPES] =
{   
    { NUM, "NUM", "#E3F2FD" },                          // USER!!!!!!!!!!!
    { VAR, "VAR", "#43A047" },                          // DONT CHANGE THE ORDER!!!!!
    {  OP,  "OP", "#FFF3E0" }                           // GET THE FUCK OUT!!!!!!!!!
};


// -----------------
//   GENERAL TOOLS
// -----------------

Node*        NewNode             (NodeType node_type, NodeVal val, Node* left, Node* right);
 
int          NodeDestroy         (Node* node);

int          TreeInit            (Tree* tree);

int          TreeDestroy         (Tree* tree);

bool         IsLeaf              (Node* node);

Node*        CopySubtree         (Node* root);

void         PrintTree           (Node* root);
    
void         FprintTree          (FILE* tree_file, Node* root);

// ------------------
// GRAPH DUMP SECTION
// ------------------

int             FormatNodeVal           (const Node* node, char* val_buff);
        
int             TreeGraphDump           (Tree* tree, const char* html_filename, 
                                         const char* file, int line, const char* func);
                                 
#endif
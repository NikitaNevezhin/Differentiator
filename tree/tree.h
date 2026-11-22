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
#define TOTAL_OPS 4
#define TOTAL_VARS 3

#define TREE_GRAPH_DUMP(tree, html_filename) TreeGraphDump(tree, html_filename, __FILE__, __LINE__, __func__);

// -------------------------------------------
// DSL for convinient work with function diff
// -------------------------------------------

#define _NUM(val)           NewNode(NUM, val, NULL, NULL)
#define _VAR(val)           NewNode(VAR, val, NULL, NULL)

#define _ADD(left, right)   NewNode(OP, (double)ADD, left, right)
#define _SUB(left, right)   NewNode(OP, (double)SUB, left, right)
#define _MUL(left, right)   NewNode(OP, (double)MUL, left, right)
#define _DIV(left, right)   NewNode(OP, (double)DIV, left, right)


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
    DIV
};

struct NodeVal
{
    double num_val;
    OpType op_type;
    int var_idx;
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

const char VARS[TOTAL_VARS] =
{
    'x', 'y', 'z'
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

struct OpInfo
{
    OpType op;
    char symbol;
    const char* op_name;
};

const OpInfo OP_TABLE[TOTAL_OPS] =
{
    {ADD, '+', "ADD"},              // USER!!!!!!!!!!!
    {SUB, '-', "SUB"},              // DONT CHANGE THE ORDER!!!!!
    {MUL, '*', "MUL"},              // GET THE FUCK OUT!!!!!!!!!!
    {DIV, '/', "DIV"}               // please
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

int             GenerateNodeDump        (FILE* dot_file, const Node* node, int node_idx, int dump_counter);
        
int             GenerateTreeDump        (FILE* dot_file, const Node* root, int* node_idx, int dump_counter);
        
int             GenerateDotFile         (Node* root, const char* dot_filename, int dump_counter);
        
int             TreeGenerateHtmlFile    (Tree* tree,  const char* html_filename, 
                                         const char* file, int line, const char* func);
        
int             TreeGraphDump           (Tree* tree, const char* html_filename, 
                                         const char* file, int line, const char* func);
                                 
#endif
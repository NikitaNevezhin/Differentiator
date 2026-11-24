#include "diff.h"

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

// --------------------------------
//        HELPER FUNCTIONS
// --------------------------------

static bool     ContainsDiffVar             (const Node* node, VarIdx diff_var);

static int      FoldConstTriangle           (Node* parent);

static Node*    FoldConstants               (Node* node, bool* is_changed);

static int      ReplaceWithChild            (Node* parent, Node* alive, Node* dead);

static bool     CollapseNeutralTriangle       (Node* node);

static int      CollapseTriangleToZero      (Node* node);

static Node*    RemoveNeutralElements       (Node* node, bool* is_changed);



static bool ContainsDiffVar(const Node* node, VarIdx diff_var)
{
    if (node == NULL) return false;

    if (node->node_type == VAR) return node->val.var_idx == diff_var;

    return ContainsDiffVar(node->left, diff_var) || ContainsDiffVar(node->right, diff_var);
}


Node* Diff(const Node* node, VarIdx diff_var)
{
    if (node == NULL) return NULL;

    switch (node->node_type)
    {
        case NUM: return _NUM(0);
        
        case VAR: 
        {   
            if (node->val.var_idx == diff_var) return _NUM(1);
            else return _NUM(0);
        }
        case OP:
        {
            switch (node->val.op_type)
            {
                case ADD: return _ADD(_DL, _DR);

                case SUB: return _SUB(_DL, _DR);

                case MUL: return _ADD(_MUL(_DL, _CR), _MUL(_CL, _DR));

                case DIV: return _DIV(_SUB(_MUL(_DL, _CR), _MUL(_CL, _DR)), _MUL(_CR, _CR));

                case POW:
                {
                    if (ContainsDiffVar(node->right, diff_var))
                    {
                        PRINT_RED("Exponent functions diff is still in process!\n");
                        return NULL;
                    }

                    if (ContainsDiffVar(node->left, diff_var))
                    {
                        // diff(f^c) = diff(f) * f ^ (c - 1)
                        return _MUL(_DL, _MUL(_CR, _POW(_CL, _SUB(_CR, _NUM(1)))));
                    }

                    return _NUM(0); // const ^ const
                }

                default: return NULL;
            }
        }

        default: return NULL;
    }

    return NULL;
}


static int FoldConstTriangle(Node* parent)
{
    assert(parent);

    OpType op = parent->val.op_type;

    double a = parent->left->val.num_val;
    double b = parent->right->val.num_val;

    if (op == DIV && IsFloatEqual(b, 0.0))
    {
        PRINT_RED("Zero division error in %s\n", __func__);
        return 1;
    }

    free(parent->left);
    free(parent->right);

    parent->left = NULL;
    parent->right = NULL;
    parent->node_type = NUM;

    switch(op)
    {
        case ADD: parent->val.num_val = a + b;
                  break;
        
        case SUB: parent->val.num_val = a - b;
                  break;

        case MUL: parent->val.num_val = a * b;
                  break;

        case DIV: parent->val.num_val = a / b;
                  break;

        case POW: parent->val.num_val = pow(a, b);
                  break;

        default: return 1;
    }

    return 0;
}


static Node* FoldConstants(Node* node, bool* is_changed)
{
    if (node == NULL) return NULL;

    // base case
    if (node->node_type != OP) return node;

    // recursive case
    if (node->left->node_type == OP)  FoldConstants(node->left, is_changed);
    if (node->right->node_type == OP) FoldConstants(node->right, is_changed);

    if (node->left->node_type == NUM && node->right->node_type == NUM)
    {
        FoldConstTriangle(node);
        *is_changed = true;
        return node;
    }

    return node;
}

static int CollapseTriangleToZero(Node* node)
{
    assert(node);

    NodeDestroy(node->left);
    NodeDestroy(node->right);

    node->node_type = NUM;
    node->val.num_val = 0;
    node->val.op_type = OP_NONE;
    node->left = NULL;
    node->right = NULL;

    return 0;
}

static int ReplaceWithChild(Node* parent, Node* alive, Node* dead)
{
    assert(parent);
    assert(alive);
    assert(dead);

    free(dead);

    parent->node_type = alive->node_type;
    parent->val = alive->val;
    parent->left = alive->left;
    parent->right = alive->right;

    if (parent->left) parent->left->parent = parent;
    if (parent->right) parent->right->parent = parent;

    free(alive);

    return 0;
}

static bool CollapseNeutralTriangle(Node* node)
{
    switch(node->val.op_type)
    {
        case ADD:
        {
            if (_LEFT_0)
            {
                ReplaceWithChild(node, node->right, node->left);
                return true;
            }

            else if (_RIGHT_0)
            {
                ReplaceWithChild(node, node->left, node->right);
                return true;
            }

            break;
        }

        case SUB:
        {
            if (_RIGHT_0)
            {
                ReplaceWithChild(node, node->left, node->right);
                return true;
            }

            break;
        }

        case MUL:
        {
            if (_LEFT_0 || _RIGHT_0)
            {
                CollapseTriangleToZero(node);
                return true;
            }

            if (_LEFT_1)
            {
                ReplaceWithChild(node, node->right, node->left);
                return true;
            }

            if (_RIGHT_1)
            {
                ReplaceWithChild(node, node->left, node->right);
                return true;
            }

            break;
        }

        case DIV:
        {
            if (_LEFT_0 && !(_RIGHT_0))
            {
                CollapseTriangleToZero(node);
                return true;
            }

            if (_RIGHT_1)
            {
                ReplaceWithChild(node, node->left, node->right);
                return true;
            }

            break;
        }

        case POW:
        {
            if (_RIGHT_1)
            {
                ReplaceWithChild(node, node->left, node->right);
                return true;
            }

            break;
        }

        default: break;
    }

    return false;
}

static Node* RemoveNeutralElements(Node* node, bool* is_changed)
{
    if (node == NULL) return NULL;
    // base case
    if (node->node_type != OP) return node;

    node->left = RemoveNeutralElements(node->left, is_changed);
    node->right = RemoveNeutralElements(node->right, is_changed);

    if(CollapseNeutralTriangle(node)) *is_changed = true;

    return node;
}

int SimplifyTree(Tree* tree)
{
    assert(tree);
    if (tree->root == NULL) return 0;

    bool is_changed = true;
    while (is_changed)
    {
        is_changed = false;
        tree->root = FoldConstants(tree->root, &is_changed);
        tree->root = RemoveNeutralElements(tree->root, &is_changed);
    }

    if (tree->root != NULL)
    {
        tree->root->parent = NULL;
    }

    return 0;
}
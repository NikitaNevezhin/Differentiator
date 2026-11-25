#include "simplifier.h"
#include "dsl.h"
#include "../utils/FloatTools.h"
#include "../common/op_table.h"

static int      FoldConstTriangle           (Node* parent);

static Node*    FoldConstants               (Node* node, bool* is_changed);

static int      ReplaceWithChild            (Node* parent, Node* alive, Node* dead);

static bool     CollapseNeutralTriangle     (Node* node);

static int      CollapseTriangleToZero      (Node* node);

static Node*    RemoveNeutralElements       (Node* node, bool* is_changed);


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

    int args = OP_TABLE[node->val.op_type].arg_amount;

    if (args == 1) return node;

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
    if (node->right) node->right = RemoveNeutralElements(node->right, is_changed);

    if(CollapseNeutralTriangle(node)) *is_changed = true;

    return node;
}

int SimplifyTree(Tree* tree)
{
    PRINT_GREEN("Starting in %s\n", __func__);
    assert(tree);
    if (tree->root == NULL) return 0;

    PRINT_GREEN("Successful execution before while in %s\n", __func__);
    bool is_changed = true;
    while (is_changed)
    {
        is_changed = false;
        PRINT_GREEN("Before executing FoldConstants\n");
        tree->root = FoldConstants(tree->root, &is_changed);
        PRINT_GREEN("FoldConstants executed successfully\n");
        tree->root = RemoveNeutralElements(tree->root, &is_changed);
        PRINT_GREEN("RemoveNeutralElements executed successfully\n");

    }

    if (tree->root != NULL)
    {
        tree->root->parent = NULL;
    }

    return 0;
}

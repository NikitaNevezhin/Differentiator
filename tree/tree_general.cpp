#include "tree.h"

Node* NewNode(NodeType node_type, double val, Node* left, Node* right)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    if (new_node == NULL)
    {
        PRINT_RED("callocation failure in %s\n", __func__);
        return NULL;
    }

    new_node->node_type = node_type;
    new_node->val = val;
    new_node->left = left;
    new_node->right = right;

    if (left != NULL)  left->parent = new_node;
    if (right != NULL) right->parent = new_node;

    return new_node;
}

int NodeDestroy(Node* node)
{
    if (node == NULL) return 0;  // suppose it is destroyed

    NodeDestroy(node->left);
    NodeDestroy(node->right);

    node->val = POIZON;

    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    free(node);

    return 0;
}

int TreeInit(Tree* tree)
{
    assert(tree);

    tree->root = NULL;
    tree->total_nodes = 0;
 
    return 0;
}

int TreeDestroy(Tree* tree)
{
    if (tree == NULL) return 0;  // suppose it is already destroyed

    NodeDestroy(tree->root);

    tree->total_nodes = 0;

    return 0;
}

int LinkLeft(Node* parent, Node* child)
{
    assert(parent);

    parent->left = child;

    return 0;
}

int LinkRight(Node* parent, Node* child)
{
    assert(parent);

    parent->right = child;

    return 0;
}

bool IsLeaf(Node* node)
{
    assert(node);

    return (node->left == NULL && node->right == NULL);
}
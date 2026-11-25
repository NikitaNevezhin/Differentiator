#include "tree.h"

#include "../common/op_table.h"

Node* NewNode(NodeType node_type, NodeVal val, Node* left, Node* right)
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

bool IsLeaf(Node* node)
{
    assert(node);

    return (node->left == NULL && node->right == NULL);
}

Node* CopySubtree(Node* node)
{
    if (node == NULL) return NULL;

    Node* left_subtree = CopySubtree(node->left);
    Node* right_subtree = CopySubtree(node->right);

    return NewNode(node->node_type, node->val, left_subtree, right_subtree);
}

void PrintTree(Node* root)
{
    assert(root);

    char val_buff[VAL_MAX_SIZE] = {};
    FormatNodeVal(root, val_buff);

    printf("(");
    printf("%s", val_buff);

    if (root->left) PrintTree(root->left);
    else            printf("()");

    if (root->right) PrintTree(root->right);
    else            printf("()");

    printf(")");
}

void FprintTree(FILE* tree_file, Node* root)
{
    assert(tree_file);

    char val_buff[VAL_MAX_SIZE] = {};
    FormatNodeVal(root, val_buff);

    fprintf(tree_file, "(");
    if (root->node_type == OP)
        fprintf(tree_file, "%s", OP_TABLE[root->val.op_type].designation);
    
    else
        fprintf(tree_file, "%s", val_buff);

    if (root->left) FprintTree(tree_file, root->left);
    // else            fprintf(tree_file, "()");

    if (root->right) FprintTree(tree_file, root->right);
    // else             fprintf(tree_file, "()");

    fprintf(tree_file, ")");
}
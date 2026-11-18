#include "diff.h"

bool IsOperator(const char* val)
{
    assert(val);

    if (strlen(val) != 1) return false;

    char operators[] = {'+', '-', '*', '/'};

    for (size_t i = 0; i < sizeof(operators) / sizeof(char); i++)
    {
        if (val[0] == operators[i]) return true;
    }

    return false;
}

bool IsVariable(const char* val)
{
    assert(val);

    if (strlen(val) != 1) return false;

    char variables[] = {'x', 'y', 'z'};

    for (size_t i = 0; i < sizeof(variables) / sizeof(char); i++)
    {
        if (val[0] == variables[i]) return true;
    }

    return false;
}

bool IsNumber(const char* val)
{
    assert(val);

    return IsStrDouble(val);
}

int ParseData(char* val_buff, FILE* tree_file)
{
    assert(val_buff);
    assert(tree_file);

    int curr_val_idx = 0;
    int  curr_ch = fgetc(tree_file);

    while (curr_ch != EOF && curr_ch != '(' && curr_ch != ')')
    {
        val_buff[curr_val_idx] = (char)curr_ch;
        curr_val_idx++;
        curr_ch = fgetc(tree_file);
    }

    val_buff[curr_val_idx] = '\0';

    ungetc(curr_ch, tree_file);

    return 0;
}

int GetNodeInfo(const char* val_buff, NodeType* node_type, double* val)
{
    assert(val_buff);
    assert(node_type);
    assert(val);

    if (IsStrDouble(val_buff))
    {   
        *node_type = NUM;
        *val = strtod(val_buff, NULL);
        return 0;
    }

    if (strlen(val_buff) == 1)  // operator or variable
    {
        char symb = val_buff[0];

        for (int i = 0; i < TOTAL_OPS; i++)
        {
            if (OP_TABLE[i].symbol == symb)
            {
                *node_type = OP;
                *val = i;
                return 0;
            }
        }

        for (int i = 0; i < TOTAL_VARS; i++)
        {
            if (VARS[i] == symb)
            {
                *node_type = VAR;
                *val = i;
                return 0;
            }
        }
    }

    return 1;  // could not get value
}

Node* ParseSubtree(FILE* tree_file, int* total_nodes)
{
    assert(tree_file);
    
    int first_symb = fgetc(tree_file);

    if (first_symb == '(')
    {   
        int second_symb = fgetc(tree_file);
        if (second_symb == ')') return NULL;  // empty subtree
        ungetc(second_symb, tree_file);

        (*total_nodes)++;

        char val_buff[256] = {};
        ParseData(val_buff, tree_file);

        NodeType node_type = NUM;
        double val = 0;
        GetNodeInfo(val_buff, &node_type, &val);

        Node* left_subtree = ParseSubtree(tree_file, total_nodes);
        Node* right_subtree = ParseSubtree(tree_file, total_nodes);

        int close_symb = fgetc(tree_file);

        if (close_symb != ')') 
        {
            PRINT_RED("Parsing ERROR: expected ')', got '%c'\n", close_symb);
            return NULL;
        }

        return NewNode(node_type, val, left_subtree, right_subtree);
    }

    if (first_symb != EOF) ungetc(first_symb, tree_file);

    return NULL;
}

int ParseTree(Tree* tree, const char* tree_filename)
{
    assert(tree);
    assert(tree_filename);

    FILE* tree_file = fopen(tree_filename, "r");

    if (tree_file == NULL)
    {
        PRINT_RED("fopen failed in %s\n", __func__);
        return 1;
    }

    tree->root = ParseSubtree(tree_file, &tree->total_nodes);

    fclose(tree_file);

    return 0;
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
        fprintf(tree_file, "%c", OP_TABLE[(int)root->val].symbol);
    
    else
        fprintf(tree_file, "%s", val_buff);

    if (root->left) FprintTree(tree_file, root->left);
    // else            fprintf(tree_file, "()");

    if (root->right) FprintTree(tree_file, root->right);
    // else             fprintf(tree_file, "()");

    fprintf(tree_file, ")");
}
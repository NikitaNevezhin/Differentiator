#include "diff.h"

int ParseData(char* val_buff, FILE* tree_file)
{
    assert(val_buff);
    assert(tree_file);

    SkipSpaces(tree_file);

    int curr_val_idx = 0;
    int  curr_ch = fgetc(tree_file);

    while (curr_ch != EOF && curr_ch != '(' && curr_ch != ')' && curr_ch != '+' && curr_ch != '-' && curr_ch != '*' && curr_ch != '/' && curr_ch != ' ')
    {  // TODO: too much of hard code
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

Node* ParsePrefixSubtree(FILE* tree_file, int* total_nodes)
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

        Node* left_subtree = ParsePrefixSubtree(tree_file, total_nodes);
        Node* right_subtree = ParsePrefixSubtree(tree_file, total_nodes);

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

Node* ParseInfixSubtree(FILE* tree_file, int* total_nodes)
{
    assert(tree_file);

    SkipSpaces(tree_file);
    int first_symb = fgetc(tree_file);

    // (left_shit) op (right_shit)
    // where (left_shit) is (left_left_shit) op (right_left_shit)
    // etc

    // RECURSIVE CASE

    if (first_symb == '(')
    {
        SkipSpaces(tree_file);
        Node* left_subtree = ParseInfixSubtree(tree_file, total_nodes);

        SkipSpaces(tree_file);
        char op_buff[VAL_MAX_SIZE] = {};
        int op_symb = fgetc(tree_file);
        op_buff[0] = op_symb;

        NodeType node_type = OP;
        double val = 0;
        GetNodeInfo(op_buff, &node_type, &val);

        SkipSpaces(tree_file);
        Node* right_subtree = ParseInfixSubtree(tree_file, total_nodes);

        int close_symb = fgetc(tree_file);

        if (close_symb != ')') 
        {
            PRINT_RED("Parsing ERROR: expected ')', got '%c'\n", close_symb);
            return NULL;
        }

        (*total_nodes)++;
        return NewNode(node_type, val, left_subtree, right_subtree);
    }

    // BASE CASE (NUM and VAR are always leaves)

    ungetc(first_symb, tree_file);

    char val_buff[256] = {};
    ParseData(val_buff, tree_file);
    SkipSpaces(tree_file);

    NodeType node_type = NUM;
    double val = 0;
    GetNodeInfo(val_buff, &node_type, &val);

    (*total_nodes)++;
    return NewNode(node_type, val, NULL, NULL);
}

int ParsePrefixTree(Tree* tree, const char* tree_filename)
{
    assert(tree);
    assert(tree_filename);

    FILE* tree_file = fopen(tree_filename, "r");

    if (tree_file == NULL)
    {
        PRINT_RED("fopen failed in %s\n", __func__);
        return 1;
    }

    tree->root = ParsePrefixSubtree(tree_file, &tree->total_nodes);

    fclose(tree_file);

    return 0;
}

int ParseInfixTree(Tree* tree, const char* tree_filename)
{
    assert(tree);
    assert(tree_filename);

    FILE* tree_file = fopen(tree_filename, "r");

    if (tree_file == NULL)
    {
        PRINT_RED("fopen failed in %s\n", __func__);
        return 1;
    }

    tree->root = ParseInfixSubtree(tree_file, &tree->total_nodes);

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
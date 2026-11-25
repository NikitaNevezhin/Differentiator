#include "expr_parser.h"
#include "lexer.h"

#include "../common/op_table.h"

// --------------------------------
//        HELPER FUNCTIONS
// --------------------------------

static Node*        ParseExpression        (TokenList* token_list, int* total_nodes);

static Node*        ParseTerm              (TokenList* token_list, int* total_nodes);

static Node*        ParseUnary             (TokenList* token_list, int* total_nodes);

static Node*        ParsePower             (TokenList* token_list, int* total_nodes);

static Node*        ParseFactor            (TokenList* token_list, int* total_nodes);


static Node* ParseFactor(TokenList* token_list, int* total_nodes)
{
    assert(token_list);
    assert(total_nodes);

    Token curr_token = token_list->data[token_list->pos];

    switch (curr_token.token_type)
    {
        case TOKEN_NUM:
        {
            token_list->pos++;
            (*total_nodes)++;
            return NewNode(NUM, curr_token.token_value, NULL, NULL);
        }

        case TOKEN_VAR:
        {
            token_list->pos++;
            (*total_nodes)++;
            return NewNode(VAR, curr_token.token_value, NULL, NULL);
        }

        case TOKEN_LPAREN:
        {
            token_list->pos++;
            Node* node = ParseExpression(token_list, total_nodes);

            if (node == NULL) return NULL;
            
            // if expression was parsed but its opening paren was never closed
            if (token_list->data[token_list->pos].token_type != TOKEN_RPAREN)
            {
                PRINT_RED("Syntax error in %s: expected ')' on pos %d\n", __func__, token_list->pos);
                return NULL;
            }

            token_list->pos++;
            return node;
        }

        case TOKEN_OP:
        {
            OpType curr_op = curr_token.token_value.op_type;
            
            if (OP_TABLE[curr_op].arg_amount == 1)
            {
                token_list->pos++;
                if (token_list->data[token_list->pos].token_type != TOKEN_LPAREN)
                {
                    PRINT_RED("Syntax error in %s: expected '(' after sin\n", __func__);
                    return NULL;
                }

                token_list->pos++;
                Node* sin_arg = ParseExpression(token_list, total_nodes);

                if (sin_arg == NULL) return NULL;

                if (token_list->data[token_list->pos].token_type != TOKEN_RPAREN)
                {
                    PRINT_RED("Syntax error in %s: expected ')' on pos %d\n", __func__, token_list->pos);
                    return NULL;
                }

                token_list->pos++;
                (*total_nodes)++;
                return NewNode(OP, {NAN, SIN, VAR_NONE}, sin_arg, NULL);
            }

            PRINT_RED("Got binary operation in ParseFactor - some syntax error\n");
            return NULL;
        }

        default:
        {
            PRINT_RED("IDK, some error in %s, I am tired of writing errors\n", __func__);
            return NULL;
        }
    }
}

static Node* ParsePower(TokenList* token_list, int* total_nodes)
{
    assert(token_list);
    assert(total_nodes);

    Node* left = ParseFactor(token_list, total_nodes);
    if (left == NULL) return NULL;

    if (IsCurrTokenOp(token_list, POW))
    {
        NodeVal op_val = token_list->data[token_list->pos].token_value;
        token_list->pos++;

        Node* right = ParsePower(token_list, total_nodes);
        if (right == NULL) return NULL;

        (*total_nodes)++;
        left = NewNode(OP, op_val, left, right);
    }

    return left;
}

static Node* ParseUnary(TokenList* token_list, int* total_nodes)
{
    assert(token_list);
    assert(total_nodes);

    if (token_list->data[token_list->pos].token_type == TOKEN_OP &&
        token_list->data[token_list->pos].token_value.op_type == SUB)
    {
        token_list->pos++;
        return NewNode(OP,  {NAN, SUB, VAR_NONE},  NewNode(NUM, {0, OP_NONE, VAR_NONE}, NULL, NULL), ParsePower(token_list, total_nodes));
    }

    return ParsePower(token_list, total_nodes);
}

static Node* ParseTerm(TokenList* token_list, int* total_nodes)
{
    assert(token_list);
    assert(total_nodes);

    Node* left = ParseUnary(token_list, total_nodes);
    if (left == NULL) return NULL;

    while (IsCurrTokenOp(token_list, DIV) || IsCurrTokenOp(token_list, MUL))
    {
        NodeVal op_val = token_list->data[token_list->pos].token_value;
        token_list->pos++;

        Node* right = ParseUnary(token_list, total_nodes);
        if (right == NULL) return NULL;

        (*total_nodes)++;
        left = NewNode(OP, op_val, left, right);
    }

    return left;
}

static Node* ParseExpression(TokenList* token_list, int* total_nodes)
{
    assert(token_list);
    assert(total_nodes);

    Node* left = ParseTerm(token_list, total_nodes);
    if (left == NULL) return NULL;

    while (IsCurrTokenOp(token_list, ADD) || IsCurrTokenOp(token_list, SUB))
    {
        NodeVal op_val = token_list->data[token_list->pos].token_value;
        token_list->pos++;

        Node* right = ParseTerm(token_list, total_nodes);
        if (right == NULL) return NULL;

        (*total_nodes)++;
        left = NewNode(OP, op_val, left, right);
    }

    return left;
}

int ParseTree(Tree* tree, const char* tree_filename)
{
    if (tree == NULL) PRINT_RED("tree is NULL!\n");
    assert(tree);
    assert(tree_filename);

    TokenList token_list = {};
    TokenizeFile(tree_filename, &token_list);

    tree->root = ParseExpression(&token_list, &tree->total_nodes);

    TokenListDtor(&token_list);

    return 0;
}
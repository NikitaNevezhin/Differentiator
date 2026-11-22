#include "expr_parser.h"

// --------------------------------
//        HELPER STRUCTURES
// --------------------------------

enum TokenType
{
    TOKEN_NUM,
    TOKEN_VAR,
    TOKEN_OP,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END
};

struct Token
{
    TokenType token_type;
    NodeVal token_value;
};

struct TokenList
{
    Token* data;  // all tokens
    int size;     // actual amount of tokens
    int capacity; // memory allocated for data
    int pos;      // token, parser currently works on
};

// --------------------------------
//        HELPER FUNCTIONS
// --------------------------------

static int          TokenListCtor          (TokenList* token_list, int capacity);

static int          TokenListDtor          (TokenList* token_list);

static bool         IsCurrTokenOp          (TokenList* token_list, OpType target_op);

static int          TokenCtor              (Token* token, const char* val_buff);

static int          Tokenize               (const char* file_buff, TokenList* token_list);

static Node*        ParseExpression        (TokenList* token_list, int* total_nodes);

static Node*        ParseTerm              (TokenList* token_list, int* total_nodes);

static Node*        ParsePower             (TokenList* token_list, int* total_nodes);

static Node*        ParseFactor            (TokenList* token_list, int* total_nodes);


int TokenCtor(Token* token, const char* val_buff)
{
    assert(val_buff);
    assert(token);

    if (IsStrDouble(val_buff))
    {   
        token->token_type = TOKEN_NUM;
        token->token_value.num_val = strtod(val_buff, NULL);
        return 0;
    }

    if (strlen(val_buff) == 1)  // operator or variable
    {
        char symb = val_buff[0];

        if (symb == '(') 
        {
            token->token_type = TOKEN_LPAREN;
            return 0;
        }

        if (symb == ')')
        {
            token->token_type = TOKEN_RPAREN;
            return 0;
        }

        for (int i = 0; i < TOTAL_OPS; i++)
        {
            if (OP_TABLE[i].symbol == symb)
            {
                token->token_type = TOKEN_OP;
                token->token_value.op_type = OP_TABLE[i].op;
                return 0;
            }
        }

        for (int i = 0; i < TOTAL_VARS; i++)
        {
            if (VAR_TABLE[i].symb == symb)
            {
                token->token_type = TOKEN_VAR;
                token->token_value.var_idx = VAR_TABLE[i].idx;
                return 0;
            }
        }
    }

    return 1;  // could not get value
}

int TokenListCtor(TokenList* token_list, int capacity)
{
    assert(token_list);

    Token* tokens_data = (Token*)calloc(capacity, sizeof(Token));

    if (tokens_data == NULL)
    {
        PRINT_RED("callocation failure in %s\n", __func__);
        return 1;
    }

    token_list->data = tokens_data;
    token_list->size = 0;
    token_list->capacity = capacity;
    token_list->pos = 0;

    return 0;
}

int TokenListDtor(TokenList* list)
{
    assert(list);

    free(list->data);
    list->data     = NULL;
    list->size     = 0;
    list->capacity = 0;
    list->pos      = 0;

    return 0;
}

bool IsCurrTokenOp(TokenList* token_list, OpType target_op)
{
    assert(token_list);

    Token curr = token_list->data[token_list->pos];
    return (curr.token_type == TOKEN_OP && curr.token_value.op_type == target_op);
}

int Tokenize(const char* file_buff, TokenList* token_list)
{
    assert(file_buff);
    assert(token_list);

    token_list->size = 0;
    token_list->pos = 0;

    while (*file_buff != '\0')
    {
        Token* curr_token = &token_list->data[token_list->size];

        if (isdigit(*file_buff) || *file_buff == '.')  // cases like 3.1.4 will be processed by in TokenCtor in IsStrDouble
        {
            char num_buff[256] = {};
            int curr_idx = 0;
            while (isdigit(*file_buff) || *file_buff == '.')
            {
                num_buff[curr_idx] = *file_buff;
                curr_idx++;
                file_buff++;
            }
            num_buff[curr_idx] = '\0';

            curr_token->token_type = TOKEN_NUM;
            curr_token->token_value.num_val = strtod(num_buff, NULL);
            token_list->size++;
            continue;
        }

        else
        {
            char val_buff[] = {*file_buff, '\0'};
            if (TokenCtor(curr_token, val_buff) == 1)
            {
                PRINT_RED("Could not read token %c in %s\n", *file_buff, __func__);
                return 1;
            }

            file_buff++;
            token_list->size++;
        }
    }

    token_list->data[token_list->size].token_type = TOKEN_END;

    return 0;
}

Node* ParseFactor(TokenList* token_list, int* total_nodes)
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

        default:
        {
            PRINT_RED("IDK, some error in %s, I am tired of writing errors\n", __func__);
            return NULL;
        }
    }
}

Node* ParsePower(TokenList* token_list, int* total_nodes)
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

Node* ParseTerm(TokenList* token_list, int* total_nodes)
{
    assert(token_list);
    assert(total_nodes);

    Node* left = ParsePower(token_list, total_nodes);
    if (left == NULL) return NULL;

    while (IsCurrTokenOp(token_list, DIV) || IsCurrTokenOp(token_list, MUL))
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

Node* ParseExpression(TokenList* token_list, int* total_nodes)
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
    assert(tree);
    assert(tree_filename);

    int size = GetFileSize(tree_filename);

    char* file_buff = (char*)calloc(size + 1, sizeof(char));
    
    if (file_buff == NULL)
    {
        PRINT_RED("callocation failure in %s\n", __func__);
        return 1;
    }

    ReadFromFile(tree_filename, file_buff, size);
    StripSpaces(file_buff);

    TokenList token_list = {};
    TokenListCtor(&token_list, size + 1);

    Tokenize(file_buff, &token_list);

    free(file_buff);

    tree->root = ParseExpression(&token_list, &tree->total_nodes);

    TokenListDtor(&token_list);

    return 0;
}
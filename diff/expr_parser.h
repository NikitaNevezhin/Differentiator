#ifndef EXPR_PARSER_H

#define EXPR_PARSER_H

#include "diff.h"

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
    int size;
    int capacity;
    int pos;      // token parser currently works on
};

int          TokenListCtor          (TokenList* token_list, int capacity);

int          TokenListDtor          (TokenList* token_list);

bool         IsCurrTokenOp          (TokenList* token_list, OpType target_op);

int          TokenCtor              (Token* token, const char* val_buff);

int          Tokenize               (const char* file_buff, TokenList* token_list);

Node*        ParseExpression        (TokenList* token_list, int* total_nodex);

Node*        ParseTerm              (TokenList* token_list, int* total_nodex);

Node*        ParseFactor            (TokenList* token_list, int* total_nodex);

int          ParseTree              (Tree* tree, const char* filename);

#endif
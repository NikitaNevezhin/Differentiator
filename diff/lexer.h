#ifndef LEXER_H

#define LEXER_H

#include "../tree/tree.h"

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


int          TokenListDtor          (TokenList* token_list);

bool         IsCurrTokenOp          (TokenList* token_list, OpType target_op);

int          TokenizeFile           (const char* filename, TokenList* token_list);

#endif
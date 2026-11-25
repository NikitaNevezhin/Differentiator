#include "lexer.h"

#include "../common/op_table.h"
#include "../utils/FileTools.h"

static int          Tokenize               (const char* file_buff, TokenList* token_list);

static int          TokenCtor              (Token* token, const char* val_buff);

static int          TokenListCtor          (TokenList* token_list, int capacity);


static int TokenCtor(Token* token, const char* val_buff)
{
    assert(val_buff);
    assert(token);

    if (IsStrDouble(val_buff))
    {   
        token->token_type = TOKEN_NUM;
        token->token_value.num_val = strtod(val_buff, NULL);
        return 0;
    }

    if (strlen(val_buff) > 0)  // operator or variable
    {
        if (strcmp(val_buff, "(") == 0) 
        {
            token->token_type = TOKEN_LPAREN;
            return 0;
        }

        if (strcmp(val_buff, ")") == 0)
        {
            token->token_type = TOKEN_RPAREN;
            return 0;
        }

        for (int i = 0; i < TOTAL_OPS; i++)
        {
            if (strcmp(val_buff, OP_TABLE[i].designation) == 0)
            {
                token->token_type = TOKEN_OP;
                token->token_value.op_type = OP_TABLE[i].op;
                return 0;
            }
        }

        for (int i = 0; i < TOTAL_VARS; i++)
        {
            if (strcmp(val_buff, VAR_TABLE[i].name) == 0)
            {
                token->token_type = TOKEN_VAR;
                token->token_value.var_idx = VAR_TABLE[i].idx;
                return 0;
            }
        }
    }

    return 1;  // could not get value
}

static int TokenListCtor(TokenList* token_list, int capacity)
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

static int Tokenize(const char* file_buff, TokenList* token_list)
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

        else if (isalpha(*file_buff))
        {
            char word_buff[256] = {};
            int curr_idx = 0;
            while (isalpha(*file_buff))
            {
                word_buff[curr_idx] = *file_buff;
                curr_idx++;
                file_buff++;
            }

            word_buff[curr_idx] = '\0';

            if (TokenCtor(curr_token, word_buff) != 0)
            {
                PRINT_RED("Unknown word '%s' in %s\n", word_buff, __func__);
                return 1;
            }

            token_list->size++;
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

int TokenizeFile(const char* filename, TokenList* token_list)
{
    assert(filename);
    assert(token_list);

    int size = GetFileSize(filename);

    char* file_buff = (char*)calloc(size + 1, sizeof(char));

    if (file_buff == NULL)
    {
        PRINT_RED("callocation failure in %s\n", __func__);
        return 1;
    }

    ReadFromFile(filename, file_buff, size);
    StripSpaces(file_buff);

    TokenListCtor(token_list, size + 1);

    Tokenize(file_buff, token_list);

    free(file_buff);

    return 0;
}
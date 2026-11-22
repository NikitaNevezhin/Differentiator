#include "StringTools.h"

int IgnoreCaseStrcmp(const char* s1, const char* s2)
{
    int i = 0;

    while (tolower(s1[i]) == tolower(s2[i]) && s1[i] != '\0')
        i++;

    return (int)(s1[i]) - (int)(s2[i]);
}

bool IsStrDouble(const char *s) 
{
    if (s == NULL || *s == '\0') {
        return false;
    }

    errno = 0;
    char *endptr = NULL;
    strtod(s, &endptr);

    if (endptr == s || *endptr != '\0') 
    {
        return false;
    }

    return true;
}

bool IsSpace(const char symb)
{
    return (symb == ' ' || symb == '\t');
}

int SkipSpaces(FILE* file)
{
    assert(file);

    int curr_symb = fgetc(file);

    while (curr_symb != EOF && IsSpace(curr_symb))
        curr_symb = fgetc(file);

    ungetc(curr_symb, file);
    
    return 0;
}

int StripSpaces(char* buff)
{
    assert(buff);

    char* write_ptr = buff;
    char* read_ptr = buff;

    while (*read_ptr != '\0')
    {
        if (!isspace(*read_ptr))
        {
            *write_ptr = *read_ptr;
            write_ptr++;
        }

        read_ptr++;
    }

    *write_ptr = '\0';

    return 0;
}
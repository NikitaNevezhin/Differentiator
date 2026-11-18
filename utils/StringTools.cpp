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
#ifndef STRING_TOOLS_H

#define STRING_TOOLS_H

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>

int     IgnoreCaseStrcmp    (const char* s1, const char* s2);

bool    IsStrDouble         (const char *str);

bool    IsSpace             (const char symb);

int     SkipSpaces          (FILE* file);

int     StripSpaces         (char* buff);


#endif
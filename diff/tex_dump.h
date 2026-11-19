#ifndef TEX_DUMP_H

#define TEX_DUMP_H

#include "diff.h"

int         TexDumpNode         (FILE* tex_file, Node* node);

int         TreeTexDump         (Tree* tree, const char* tex_filename);

#endif
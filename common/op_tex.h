#ifndef OP_TEX_H

#define OP_TEX_H

#include <stdio.h>
#include "../tree/tree.h"

// -----------------------------
//      TEX_DUMP FUNCTIONS
// -----------------------------

int   TexDumpAdd     (FILE* tex_file, Node* node);

int   TexDumpSub     (FILE* tex_file, Node* node);

int   TexDumpMul     (FILE* tex_file, Node* node);

int   TexDumpDiv     (FILE* tex_file, Node* node);

int   TexDumpPow     (FILE* tex_file, Node* node);

int   TexDumpSin     (FILE* tex_file, Node* node);

int   TexDumpCos     (FILE* tex_file, Node* node);

int   TexDumpTan     (FILE* tex_file, Node* node);

int   TexDumpCot     (FILE* tex_file, Node* node);

int   TexDumpLn      (FILE* tex_file, Node* node);

#endif
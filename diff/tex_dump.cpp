#include "tex_dump.h"
#include "../common/op_table.h"

int TexDumpNode(FILE* tex_file, Node* node)
{
    assert(tex_file);

    if (node == NULL) return 0;

    switch (node->node_type)
    {
        case NUM:
        {   
            if (node->val.num_val < 0) fprintf(tex_file, "(%lg)", node->val.num_val);

            else fprintf(tex_file, "%lg", node->val.num_val);
            
            break;
        }

        case VAR:
        {
            fprintf(tex_file, "%s", VAR_TABLE[node->val.var_idx].name);
            break;
        }

        case OP:
        {
            OpType curr_op = node->val.op_type;

            OP_TABLE[curr_op].tex_dump_func(tex_file, node);
            
            break;
        }

        default: return 1;
    }

    return 0;
}

int TreeTexDump(Tree* tree, const char* tex_filename)
{
    assert(tree);
    assert(tex_filename);

    FILE* tex_file = fopen(tex_filename, "w");

    if (tex_file == NULL)
    {
        PRINT_RED("Failed to open file in %s\n", __func__);
        return 1;
    }

    fprintf(tex_file, "\\documentclass[a4paper, 12pt]{article}\n");
    fprintf(tex_file, "\\usepackage{graphicx}\n\n");

    fprintf(tex_file, "\\begin{document}\n\n");

    fprintf(tex_file, "$");
    TexDumpNode(tex_file, tree->root);
    fprintf(tex_file, "$");

    fprintf(tex_file, "\n\n");
    fprintf(tex_file, "\\end{document}\n");

    fclose(tex_file);

    system("pdflatex -interaction=nonstopmode -output-directory=dump dump/tex_dump.tex > nul");

    return 0;
}
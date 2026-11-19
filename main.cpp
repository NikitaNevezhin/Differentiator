#include "diff/diff.h"
#include "diff/tex_dump.h"

int main(void)
{
    Tree test_tree = {};
    ParseTree(&test_tree, "data/math_expr.txt");

    TREE_GRAPH_DUMP(&test_tree, "dump/tree_dump.html");

    Tree diff_tree = {};
    TreeInit(&diff_tree);

    diff_tree.root = Diff(test_tree.root);

    TreeTexDump(&diff_tree, "dump/tex_dump.tex");

    return EXIT_SUCCESS;
}
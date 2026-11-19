#include "diff/diff.h"
#include "diff/tex_dump.h"

int main(void)
{
    Tree test_tree = {};
    ParseTree(&test_tree, "data/math_expr.txt");

    TREE_GRAPH_DUMP(&test_tree, "dump/tree_dump.html");

    printf("%lg\n", CalcTree(test_tree.root));

    TreeTexDump(&test_tree, "dump/tex_dump.tex");

    return EXIT_SUCCESS;
}
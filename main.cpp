#include "diff/diff.h"

int main(void)
{
    Tree test_tree = {};
    ParseTree(&test_tree, "data/math_expr.txt");

    TREE_GRAPH_DUMP(&test_tree, "dump/tree_dump.html");

    printf("%lg\n", CalcTree(test_tree.root));

    return EXIT_SUCCESS;
}
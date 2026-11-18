#include "tree/tree.h"

int main(void)
{
    Tree test_tree = {};
    ParseTree(&test_tree, "math_expr.txt");

    TREE_GRAPH_DUMP(&test_tree, "tree_dump.html");

    return EXIT_SUCCESS;
}
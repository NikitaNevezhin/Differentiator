#include "diff/diff.h"

int main(void)
{
    Tree test_tree = {};
    ParseTree(&test_tree, "data/math_expr.txt");

    TREE_GRAPH_DUMP(&test_tree, "dump/tree_dump.html");

    FILE* file = fopen("data/test_fprint.txt", "w");
    FprintTree(file, test_tree.root);
    fclose(file);

    return EXIT_SUCCESS;
}
#include "diff/expr_parser.h"
#include "diff/tex_dump.h"
#include "diff/calc.h"
#include "diff/diff.h"

int main(void)
{
    double var_values[TOTAL_VARS] = {1, 2, 3};

    Tree test_tree = {};
    ParseTree(&test_tree, "data/math_expr.txt");

    TREE_GRAPH_DUMP(&test_tree, "dump/tree_dump.html");

    Tree diff_tree = {};
    TreeInit(&diff_tree);

    diff_tree.root = Diff(test_tree.root);

    printf("%lg\n", CalcTree(diff_tree.root, var_values));

    TreeTexDump(&diff_tree, "dump/tex_dump.tex");

    TreeDestroy(&test_tree);
    TreeDestroy(&diff_tree);

    return EXIT_SUCCESS;
}
#include "diff/expr_parser.h"
#include "diff/tex_dump.h"
#include "diff/calc.h"
#include "diff/diff.h"

int main(void)
{
    double var_values[TOTAL_VARS] = {};
    var_values[VAR_X] = 2;
    var_values[VAR_Y] = 3;
    var_values[VAR_Z] = 5;

    Tree test_tree = {};
    ParseTree(&test_tree, "data/math_expr.txt");
    TREE_GRAPH_DUMP(&test_tree, "dump/tree_dump.html");


    Tree diff_tree = {};
    TreeInit(&diff_tree);

    diff_tree.root = Diff(test_tree.root, VAR_X);
    SimplifyTree(&diff_tree);

    TreeTexDump(&diff_tree, "dump/tex_dump.tex");

    TreeDestroy(&test_tree);
    TreeDestroy(&diff_tree);

    return EXIT_SUCCESS;
}
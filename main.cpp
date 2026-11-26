#include "diff/expr_parser.h"
#include "diff/tex_dump.h"
#include "diff/calc.h"
#include "diff/diff.h"
#include "diff/simplifier.h"

int main(void)
{
    PRINT_GREEN("Starting in main\n");
    double var_values[TOTAL_VARS] = {};
    var_values[VAR_X] = 3.1415926535 / 2;
    var_values[VAR_Y] = 3;
    var_values[VAR_Z] = 5;

    Tree test_tree = {};
    ParseTree(&test_tree, "data/math_expr.txt");
    PRINT_GREEN("Test Tree parsed successfully.\n");
    TREE_GRAPH_DUMP(&test_tree, "dump/tree_dump.html");
    PRINT_GREEN("Test tree created successfully.\n");
    TreeTexDump(&test_tree, "dump/tex_dump.tex");

    // Tree diff_tree = {};
    // TreeInit(&diff_tree);

    // diff_tree.root = Diff(test_tree.root, VAR_X);
    // PRINT_GREEN("Tree differentiated successfully!\n");
    // SimplifyTree(&diff_tree);

    printf("%lg\n", CalcTree(test_tree.root, var_values));

    // TreeTexDump(&diff_tree, "dump/tex_dump.tex");

    TreeDestroy(&test_tree);
    // TreeDestroy(&diff_tree);

    return EXIT_SUCCESS;
}
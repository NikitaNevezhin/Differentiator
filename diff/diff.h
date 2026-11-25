#ifndef DIFF_H

#define DIFF_H

#include <ctype.h>
#include <string.h>
#include <math.h>
#include "../utils/FloatTools.h"

#include "../tree/tree.h"

// -------------------------------------------
//          DIFFERENTIATOR SECTION
// -------------------------------------------

bool         ContainsDiffVar        (const Node* node, VarIdx diff_var);

Node*        Diff                   (const Node* node, VarIdx diff_var);

#endif
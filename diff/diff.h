#ifndef DIFF_H

#define DIFF_H

#include <ctype.h>
#include <string.h>
#include <math.h>
#include "../utils/FloatTools.h"
#include "../utils/FileTools.h"

#include "../tree/tree.h"

// -------------------------------------------
//          DIFFERENTIATOR SECTION
// -------------------------------------------

Node*        Diff                (const Node* node);

#endif
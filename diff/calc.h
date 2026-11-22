#ifndef CALC_H

#define CALC_H

#include "../tree/tree.h"
#include <math.h>
#include "../utils/FloatTools.h"

double       CalcTree            (Node* root, const double* var_values);

#endif
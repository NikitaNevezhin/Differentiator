#include "op_table.h"

#include <assert.h>

#include "../utils/FloatTools.h"
#include "../utils/PrintColor.h"

#include "../diff/diff.h"
#include "../diff/dsl.h"

#include "../diff/tex_dump.h"

#include "op_calc.h"
#include "op_diff.h"
#include "op_tex.h"

// USER! (and future Nikita who will do smth here)
// If you wanna add a new operation, remember to:
// 1) Add op_code in enum OpType in tree/tree.h
// 2) Add CalcOp  in op_calc.cpp
// 3) Add DiffOp  in op_diff.cpp
// 4) ADd TexOp   in op_tex.cpp
// and your command will be ready to be used

const OpInfo OP_TABLE[TOTAL_OPS] =
{
    {ADD,   "+", "ADD", 2,      CalcAdd, DiffAdd, TexDumpAdd},              // USER!!!!!!!!!!!
    {SUB,   "-", "SUB", 2,      CalcSub, DiffSub, TexDumpSub},              // DONT CHANGE THE ORDER!!!!!
    {MUL,   "*", "MUL", 2,      CalcMul, DiffMul, TexDumpMul},              // GET THE FUCK OUT!!!!!!!!!!
    {DIV,   "/", "DIV", 2,      CalcDiv, DiffDiv, TexDumpDiv},              // please
    {POW,   "^", "POW", 2,      CalcPow, DiffPow, TexDumpPow},
    {SIN, "sin", "SIN", 1,      CalcSin, DiffSin, TexDumpSin},
    {COS, "cos", "COS", 1,      CalcCos, DiffCos, TexDumpCos},
    {TAN, "tan", "TAN", 1,      CalcTan, DiffTan, TexDumpTan},
    {COT, "cot", "COT", 1,      CalcCot, DiffCot, TexDumpCot},
    {LN,   "ln",  "LN", 1,      CalcLn,   DiffLn,  TexDumpLn}
};
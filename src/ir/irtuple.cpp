#include "irtuple.h"

#include <string>

using Ident = string;

IRTuple::IRTuple(Operator op, Ident src1, Ident src2, Ident dest) :
    op_(op),
    src1_(src1),
    src2_(src2),
    dest_(dest) {}

// Getters
Operator IRTuple::op() { return op_; }
Ident IRTuple::src1() { return src1_; }
Ident IRTuple::src2() { return src2_; }
Ident IRTuple::dest() { return dest_; }

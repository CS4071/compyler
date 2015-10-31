/** Intermediate representation (quadruples)
 */

#ifndef IRTUPLE_H
#define IRTUPLE_H

#include <string>

using namespace std;

using Ident = string;

enum class Operator {
  ADD,
  AND,
  CALL,
  DIV,
  EQU,
  FADD,
  FDIV,
  FMUL,
  FSUB,
  FUNC,
  GT,
  GTE,
  JMP,
  JMPF,
  LABEL,
  LOAD,
  LT,
  LTE,
  MOD,
  MUL,
  NEG,
  NEQ,
  NOT,
  OR,
  RET,
  STORE,
  SUB,
  XOR,
  PHI
};

class IRTuple {
  Operator op_;
  Ident src1_;
  Ident src2_;
  Ident dest_;
 public:
  IRTuple(Operator op, Ident src1, Ident src2, Ident dest);

  // Getters
  Operator op();
  Ident src1();
  Ident src2();
  Ident dest();
};

#endif

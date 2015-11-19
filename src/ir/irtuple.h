/** Intermediate representation (quadruples)
 */

#ifndef IRTUPLE_H
#define IRTUPLE_H

#include <string>
#include <wchar.h>

using namespace std;

using Ident = wstring;

enum class Operator {
  ADD,
  AND,
  CALL,
  DIV,
  EQU,
  FDIV,
  FUNC,
  GT,
  GTE,
  JMP,
  JMPF,
  LABEL,
  LOAD,
  LSHIFT,
  LT,
  LTE,
  MOD,
  MUL,
  NEG,
  NEQ,
  NOT,
  OR,
  RET,
  RSHIFT,
  STORE,
  SUB,
  XOR,
  PHI,
  POW,
  NEWM
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

IRTuple* storeImmed(Ident dest, Ident val);
IRTuple* label(Ident lab);

wstring show(IRTuple tuple);

#endif

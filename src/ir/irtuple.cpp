#include "irtuple.h"

#include <string>

using Ident = wstring;

IRTuple::IRTuple(Operator op, Ident src1, Ident src2, Ident dest) :
    op_(op),
    src1_(src1),
    src2_(src2),
    dest_(dest) {}

// Returns true if the tuple is a leader (of a basic block).
bool IRTuple::isLeader() {
  switch(op_) {
  case Operator::FUNC:
  case Operator::LABEL:
    return true;
  default:
    return false;
  }
}

// Returns true if the tuple is a terminator (of a basic block).
bool IRTuple::isTerminator() {
  switch(op_) {
  case Operator::JMP:
  case Operator::JMPF:
  case Operator::RET:
    return true;
  default:
    return false;
  }
}

// Util
IRTuple* storeImmed(Ident dest, Ident val){
  return new IRTuple(Operator::STORE, val, L"", dest);
}

IRTuple* label(Ident lab){
  return new IRTuple(Operator::LABEL, L"", L"", lab);
}

// Getters
Operator IRTuple::op() { return op_; }
Ident IRTuple::src1() { return src1_; }
Ident IRTuple::src2() { return src2_; }
Ident IRTuple::dest() { return dest_; }


wstring show(IRTuple tuple){
  switch (tuple.op()){
    case (Operator::ADD):    return tuple.dest() + L" <- " + tuple.src1() + L" + " + tuple.src2();
    case (Operator::AND):    return tuple.dest() + L" <- " + tuple.src1() + L" & " + tuple.src2();
    case (Operator::CALL):   return L"CALL " + tuple.dest();
    case (Operator::DIV):    return tuple.dest() + L" <- " + tuple.src1() + L" / " + tuple.src2();
    case (Operator::EQU):    return tuple.dest() + L" <- " + tuple.src1() + L" = " + tuple.src2();
    case (Operator::FDIV):   return tuple.dest() + L" <- " + tuple.src1() + L" // " + tuple.src2();
    case (Operator::FUNC):   return L"FUNC " + tuple.dest();
    case (Operator::GT):     return tuple.dest() + L" <- " + tuple.src1() + L" > " + tuple.src2();
    case (Operator::GTE):    return tuple.dest() + L" <- " + tuple.src1() + L" >= " + tuple.src2();
    case (Operator::JMP):    return L"JMP " + tuple.dest();
    case (Operator::JMPF):   return L"JMPF " + tuple.src1() + L" " + tuple.dest();
    case (Operator::LABEL):  return tuple.dest()+ L":";
    case (Operator::LSHIFT): return tuple.dest() + L" <- " + tuple.src1() + L" << " + tuple.src2();
    case (Operator::LT):     return tuple.dest() + L" <- " + tuple.src1() + L" < " + tuple.src2();
    case (Operator::LTE):    return tuple.dest() + L" <- " + tuple.src1() + L" <= " + tuple.src2();
    case (Operator::MOD):    return tuple.dest() + L" <- " + tuple.src1() + L" % " + tuple.src2();
    case (Operator::MUL):    return tuple.dest() + L" <- " + tuple.src1() + L" * " + tuple.src2();
    case (Operator::NEG):    return tuple.dest() + L" <- ~ " + tuple.src1();
    case (Operator::NOT):    return tuple.dest() + L" <- ! " + tuple.src1();
    case (Operator::OR):     return tuple.dest() + L" <- " + tuple.src1() + L" | " + tuple.src2();
    case (Operator::RET):    return L"RET " + tuple.dest();
    case (Operator::RSHIFT): return tuple.dest() + L" <- " + tuple.src1() + L" >> " + tuple.src2();
    case (Operator::SUB):    return tuple.dest() + L" <- " + tuple.src1() + L" - " + tuple.src2();
    case (Operator::XOR):    return tuple.dest() + L" <- " + tuple.src1() + L" ^ " + tuple.src2();
    case (Operator::POW):    return tuple.dest() + L" <- " + tuple.src1() + L" ** " + tuple.src2();
    case (Operator::LOAD):   return tuple.dest() + L" <- " + tuple.src1() + L" [" + tuple.src2() + L"]";
    case (Operator::STORE):  return tuple.dest() + L" <- " + tuple.src1() + L" [" + tuple.src2() + L"]";
    case (Operator::NEWM):   return tuple.dest() + L" <- NEWM";
    default:                 return tuple.dest() + L" <- " + tuple.src1() + L" `op` " + tuple.src2();
  }
}

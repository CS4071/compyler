// Basic block.
//
// Represents a basic block as a list of the tuples in the block and
// a list of its successor blocks.

#ifndef BASIC_BLOCK_H
#define BASIC_BLOCK_H

#include "irtuple.h"
#include <string>
#include <vector>

using namespace std;

using Ident = wstring;

class BasicBlock {
  int id_;
  vector<IRTuple*> tuples_;
  vector<BasicBlock*> successors_;
 public:
  BasicBlock(int id);

  int id();
  vector<IRTuple*> getTuples();
  void appendTuple(IRTuple *tuple);
  vector<BasicBlock*> getSuccessors();
  void addSuccessor(BasicBlock *block);
};

#endif

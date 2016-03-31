#include "basic_block.h"

#include <vector>

BasicBlock::BasicBlock(int id) : id_(id) {}

int BasicBlock::id() { return id_; }

// Tuples
vector<IRTuple*> BasicBlock::getTuples() {
  return tuples_;
}

void BasicBlock::appendTuple(IRTuple *tuple) {
  tuples_.push_back(tuple);
}

// Successors
vector<BasicBlock*> BasicBlock::getSuccessors() {
  return successors_;
}

void BasicBlock::addSuccessor(BasicBlock *block) {
  successors_.push_back(block);
}

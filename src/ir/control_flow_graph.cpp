#include "control_flow_graph.h"

#include "irtuple.h"
#include "basic_block.h"
#include <map>
#include <string>
#include <vector>

ControlFlowGraph::ControlFlowGraph(vector<IRTuple*> tuples) {
  int index = 0;
  // Maps labels to their corresponding blocks, so that successors can be
  // efficiently linked after blocks have been identified.
  map<Ident, BasicBlock*> labels;
  
  // The very first instruction always starts a basic block.
  blocks_.push_back(new BasicBlock(index++));
  blocks_.back()->appendTuple(tuples[0]);
  
  for (int i = 1; i < tuples.size(); i++) {
    if (tuples[i]->isLeader()) {
      if (blocks_.back()->getTuples().size() > 0) {
        blocks_.push_back(new BasicBlock(index++));
      }
      blocks_.back()->appendTuple(tuples[i]);
      if (tuples[i]->op() == Operator::LABEL) {
        labels[tuples[i]->dest()] = blocks_.back();
        // Make new block a successor of previous block if it starts with
        // a label (and not with a function) and the previous instruction
        // was not a jump or return.
        if (i > 0 &&
            tuples[i-1]->op() != Operator::JMP &&
            tuples[i-1]->op() != Operator::RET) {
          blocks_[blocks_.size()-2]->addSuccessor(blocks_.back());
        }
      }
    }
    else {
      blocks_.back()->appendTuple(tuples[i]);
      if (tuples[i]->isTerminator()) {
        blocks_.push_back(new BasicBlock(index++));
        // Make new block a successor of previous block if it was terminated
        // by a conditional (and not a jump or return).
        if (tuples[i]->op() == Operator::JMPF) {
          blocks_[blocks_.size()-2]->addSuccessor(blocks_.back());
        }
      }
    }
    
  }

  // Add successors defined through jump instructions.
  for (auto block : blocks_) {
    if (block->getTuples().back()->op() == Operator::JMP ||
        block->getTuples().back()->op() == Operator::JMPF) {
      block->addSuccessor(labels[block->getTuples().back()->dest()]);
    }
  }
};

vector<BasicBlock*> ControlFlowGraph::getBlocks() {
  return blocks_;
}

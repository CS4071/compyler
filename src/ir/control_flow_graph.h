// Control Flow Graph
//
// Builds the control flow graph from a list of tuples by finding basic blocks
// and stores the blocks.

#ifndef CONTROL_FLOW_GRAPH_H
#define CONTROL_FLOW_GRAPH_H

#include "irtuple.h"
#include "basic_block.h"
#include <string>
#include <vector>

using namespace std;

using Ident = wstring;

class ControlFlowGraph {
  vector<BasicBlock*> blocks_;
 public:
  // Builds the control flow graph given a list of tuples.
  ControlFlowGraph(vector<IRTuple*> tuples);

  vector<BasicBlock*> getBlocks();
};

#endif

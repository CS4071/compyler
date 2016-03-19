#include "../../gen/Parser.h"
#include "../../gen/Scanner.h"
#include "../ir/basic_block.h"
#include "../ir/control_flow_graph.h"
#include "../preprocess/preprocess.h"
#include <string>

// Boost
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE  Master
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace Compyler;

struct Parse {
  Scanner *scanner;
  Parser *parser;
  Program program;
  ControlFlowGraph *cfg;

  Parse() {}
  
  ~Parse() {
    delete cfg;
    delete parser;
    delete scanner;
  }

  void init(wstring filename) {
    scanner = new Scanner(filename.c_str());
    parser = new Parser(scanner);
    parser->Parse();

    program = parser->getProgram();
    cfg = new ControlFlowGraph(program);
  }
};

BOOST_FIXTURE_TEST_SUITE(Programs, Parse)

BOOST_AUTO_TEST_CASE(program_simple) {
  init(L"src/test/data/simple.py.pre");
  BOOST_CHECK(parser->errors->count == 0);
  BOOST_CHECK(cfg->getBlocks().size() == 1);
}

BOOST_AUTO_TEST_CASE(program_if) {
  init(L"src/test/data/if.py.pre");
  BOOST_CHECK(parser->errors->count == 0);
  BOOST_CHECK(cfg->getBlocks().size() == 4);
}

BOOST_AUTO_TEST_SUITE_END()

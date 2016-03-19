#include "../../gen/Parser.h"
#include "../../gen/Scanner.h"
#include "../ir/basic_block.h"
#include "../ir/control_flow_graph.h"
#include "../preprocess/preprocess.h"
#include <fstream>
#include <iterator>
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

struct Preprocess {
  Preprocessor *pp;
  ifstream ifs1, ifs2;
  istream_iterator<char> b1, b2, e1, e2;

  Preprocess() {}
  ~Preprocess() {
    delete pp;
  }

  void init(string file_in, string file_out) {
    pp = new Preprocessor(file_in, file_out);
    pp->preprocess();

    ifs1.open(file_in + ".pre");
    ifs2.open(file_out);

    b1 = istream_iterator<char>(ifs1);
    b2 = istream_iterator<char>(ifs2);
  }
};

BOOST_FIXTURE_TEST_SUITE(Preprocessing, Preprocess)

BOOST_AUTO_TEST_CASE(preprocess_simple) {
  string file_in = "src/test/data/simple.py";
  string file_out = "tmp/simple.py.pre";
  init(file_in, file_out);
  BOOST_CHECK_EQUAL_COLLECTIONS(b1, e1, b2, e2);
}

BOOST_AUTO_TEST_CASE(preprocess_if) {
  string file_in = "src/test/data/if.py";
  string file_out = "tmp/if.py.pre";
  init(file_in, file_out);
  BOOST_CHECK_EQUAL_COLLECTIONS(b1, e1, b2, e2);
}

BOOST_AUTO_TEST_SUITE_END()

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

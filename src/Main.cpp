#include "../gen/Parser.h"
#include "../gen/Scanner.h"
#include "ir/basic_block.h"
#include "ir/control_flow_graph.h"
#include "preprocess/preprocess.h"
#include <sys/timeb.h>
#include <wchar.h>
#include <iostream>
#include <string>

using namespace std;
using namespace Compyler;

using Program = vector<IRTuple*>;

int main (int argc, char *argv[]) {

  if (argc == 2) {
    string filename(argv[1]);
    cout << "Compiling " << filename << endl;
    // Coco/R wants to get wchar_t* ...
    wchar_t *filename_wchar = coco_string_create(argv[1]);
    wstring wide_filename = wstring(filename_wchar) + L".pre";

    Preprocessor pp(filename);
    pp.preprocess();
    Scanner *scanner = new Scanner(wide_filename.c_str());
    Parser *parser = new Parser(scanner);
    parser->Parse();
    if (parser->errors->count == 0) {
      cout << "Yey, no parse errors.\n";
    }

    Program program = parser->getProgram();
    ControlFlowGraph *cfg = new ControlFlowGraph(program);
    cout << cfg->getBlocks().size() << " basic blocks identified." << endl;

    coco_string_delete(filename_wchar);

    delete parser;
    delete scanner;

  } else
    printf("-- No source file specified\n");

  return 0;
}

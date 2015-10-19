#include "../gen/Parser.h"
#include "../gen/Scanner.h"
#include <sys/timeb.h>
#include <wchar.h>
#include <iostream>

using namespace std;
using namespace Compyler;

int main (int argc, char *argv[]) {

  if (argc == 2) {

    wchar_t *fileName = coco_string_create(argv[1]);

    Scanner *scanner = new Scanner(fileName);
    Parser *parser = new Parser(scanner);
    parser->Parse();
    if (parser->errors->count == 0) {
      cout << "Yey, no parse errors.\n";
    }

    coco_string_delete(fileName);

    delete parser;
    delete scanner;

  } else
    printf("-- No source file specified\n");

  return 0;
}

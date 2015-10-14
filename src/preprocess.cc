#include "preprocess.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

const char Preprocessor::kIndent[] = "☛";
const char Preprocessor::kDedent[] = "☚";

Preprocessor::Preprocessor(string filename) {
  infile.open(filename, ios::in);
  indent_stack.push_back(0);
  
  // Captures whitespace at start of line
  rgx = "( *)([^ ].*)";
}

Preprocessor::~Preprocessor() {
  infile.close();
}

bool Preprocessor::getLine(string *line) {
  *line = "";
  string current_line;
  
  if (getline(infile, current_line)) {
    smatch match;
    regex_match(current_line, match, rgx);
    const string indent(match[1]);
    string suffix(match[2]);

    while (suffix[suffix.length() - 1] == '\\') {
      string next_line;
      getline(infile, next_line);
      suffix = suffix.substr(0, suffix.length() - 1) + next_line;
    }

    // Add INDENT/DEDENT tokens when indentation changes
    if (indent.length() > indent_stack.back() && suffix[0] != '#') {
      indent_stack.push_back(indent.length());
      *line += string(kIndent) + " ";
    }
    else if (indent.length() < indent_stack.back() && suffix[0] != '#') {
      if (onStack(indent.length(), indent_stack)) {
        while (indent_stack.back() > indent.length()) {
          indent_stack.pop_back();
          *line += string(kDedent) + " ";
        }
      }
      else {
        cout << "IndentationError: unindent does not match any outer indentation level\n";
      }
    }
    
    *line += suffix;
    
    return true;
  }
  else if (indent_stack.back() > 0) {
    // Unindents at end of file
    while (indent_stack.back() > 0) {
      indent_stack.pop_back();
      *line += string(kDedent) + " ";
    }

    return true;
  }

  return false;
}

// Returns true if <n> is on the stack, false otherwise.
// Starts at top of stack, because we expect to find things there first.
bool Preprocessor::onStack(const int &n, const vector<int> &stack) {
  for (auto i = stack.crbegin(); i != stack.crend(); i++) {
    if (*i == n)
      return true;
  }
  return false;
}

int main(int argc, char *argv[]) {
  string filename = argv[1];
  ofstream outfile;
  outfile.open(filename + ".pre", ios::out);

  Preprocessor pp(filename);
  string line;

  while (pp.getLine(&line)) {
    outfile << line << endl;
    cout << line << endl;
  }

  outfile.close();
}

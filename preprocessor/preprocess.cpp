/** Replaces whitespace with INDENT/DEDENT tokens.
 *  Run with g++-4.9 -std=c++11
 *
 *  TODO(eginhard):
 *   - handle tabs
 *   - implicit line-joining via (), [], {}, """ """
 *   - integrate into pipeline
 */

#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

using namespace std;

bool onStack(const int &n, const vector<int> &stack);

int main(int argc, char *argv[]) {
  string filename = argv[1];
  ifstream infile;
  ofstream outfile;
  infile.open(filename, ios::in);
  outfile.open(filename + ".pre", ios::out);

  // Current indentation level always at top of stack
  vector<int> stack;
  stack.push_back(0);

  // Captures whitespace at start of line
  const regex rgx("( *)([^ ].*)");
  string line;
  
  while (getline(infile, line)) {
    smatch match;
    regex_match(line, match, rgx);
    const string indent(match[1]);
    string suffix(match[2]);

    while (suffix[suffix.length() - 1] == '\\') {
      string next_line;
      getline(infile, next_line);
      suffix = suffix.substr(0, suffix.length() - 1) + next_line;
    }

    // Add INDENT/DEDENT tokens when indentation changes
    if (indent.length() > stack.back() && suffix[0] != '#') {
      stack.push_back(indent.length());
      outfile << "☛ ";
    }
    else if (indent.length() < stack.back() && suffix[0] != '#') {
      if (onStack(indent.length(), stack)) {
        while (stack.back() > indent.length()) {
          stack.pop_back();
          outfile << "☚ ";
        }
      }
      else {
        cout << "IndentationError: unindent does not match any outer indentation level\n";
      }
    }
    
    outfile << suffix << endl;
  }

  // Unindents at end of file
  while (stack.back() > 0) {
    stack.pop_back();
    outfile << "☚ ";
  }

  infile.close();
  outfile.close();
}

bool onStack(const int &n, const vector<int> &stack) {
  for (auto i = stack.crbegin(); i != stack.crend(); i++) {
    if (*i == n)
      return true;
  }
  return false;
}

/** Replaces whitespace with INDENT/DEDENT (☛/☚) tokens.
 *  Compile with g++-4.9 -std=c++11
 *
 *  Takes a *.py file and creates a *.py.pre file where whitespace is replaced
 *  by INDENT/DEDENT/NEWLINE tokens.
 *
 *  TODO(eginhard):
 *   - handle tabs
 */

#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

using namespace std;

class Preprocessor {
  static const char kIndent[];
  static const char kDedent[];
  static const char kNewline[];

  ifstream infile_;
  ofstream outfile_;
  regex rgx_;
  // Keeps track of the current indentation level.
  vector<int> indent_stack_;
  // Keeps track of nesting elements (parentheses, quotes etc.).
  vector<string> nesting_stack_;
  static map<string, string> nesting_map_;

  // Returns true if the specified <stack> contains integer <n>.
  bool onStack(const int &n, const vector<int> &stack);
  // Returns true if the current line causes an implicit line-join.
  bool implicitJoin();
  // Returns true if the current character is within a string.
  bool inString();
  string setLastChars(const char &c, const string &last_chars);
 public:
  // Initializes the preprocessor for the given Python source file.
  Preprocessor(string filename);
  ~Preprocessor();

  // Copies the *.py source file to a *.py.pre file with all indentation
  // replaced by INDENT/DEDENT tokens.
  void preprocess();
};

#endif

/** Replaces whitespace with INDENT/DEDENT (☛/☚) tokens.
 *  Compile with g++-4.9 -std=c++11
 *
 *  Takes a *.py file and creates a *.py.pre file where whitespace is replaced
 *  by INDENT/DEDENT tokens.
 *
 *  TODO(eginhard):
 *   - handle tabs
 *   - implicit line-joining via (), [], {}, """ """
 */

#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

class Preprocessor {
  static const char kIndent[];
  static const char kDedent[];
  static const char kNewline[];

  ifstream infile;
  ofstream outfile;
  regex rgx;
  vector<int> indent_stack;

  bool onStack(const int &n, const vector<int> &stack);
 public:
  Preprocessor(string filename);
  ~Preprocessor();
  void preprocess();
};

#endif

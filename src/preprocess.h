/** Replaces whitespace with INDENT/DEDENT (☛/☚) tokens.
 *  Compile with g++-4.9 -std=c++11
 *
 *  TODO(eginhard):
 *   - handle tabs
 *   - implicit line-joining via (), [], {}, """ """
 *   - integrate into pipeline
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
  
  ifstream infile;
  regex rgx;
  vector<int> indent_stack;

  bool onStack(const int &n, const vector<int> &stack);
 public:
  Preprocessor(string filename);
  ~Preprocessor();
  // Returns true if there are lines left to process, false otherwise.
  // The processed line with added indent/dedent tokens is returned via <line>. 
  bool getLine(string *line);
};

#endif

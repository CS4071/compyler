#include "preprocess.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

// Symbols indicating INDENT/DEDENT/NEWLINE tokens
const char Preprocessor::kIndent[] = "¬";
const char Preprocessor::kDedent[] = "`";
const char Preprocessor::kNewline[] = "¶";

// Expected opening element for a given closing element.
map<string, string> Preprocessor::nesting_map_ = {
  {")", "("},
  {"]", "["},
  {"}", "{"},
  {"'", "'"},
  {"\"", "\""},
  {"\"\"\"", "\"\"\""}
};

Preprocessor::Preprocessor(string filename) {
  infile_.open(filename, ios::in);
  outfile_.open(filename + ".pre", ios::out);
  indent_stack_.push_back(0);
  nesting_stack_.push_back("");

  // Captures whitespace at start of line
  rgx_ = "( *)([^ ].*)";
}

Preprocessor::~Preprocessor() {
  infile_.close();
  outfile_.close();
}

void Preprocessor::preprocess() {
  string current_line;
  int line_count = 1;
  // True if indentation of this line should be ignored.
  bool ignore_indent = false;
  while (getline(infile_, current_line)) {
    smatch match;
    regex_match(current_line, match, rgx_);
    // Prefix containing indentation.
    const string indent(match[1]);
    // Core of the line containing actual code.
    string core(match[2]);

    // Indentation is larger than previous line, add one INDENT token.
    if (!ignore_indent &&
        indent.length() > indent_stack_.back() &&
        core[0] != '#') {
      indent_stack_.push_back(indent.length());
      outfile_ << string(kIndent) << " ";
    }
    // Indentation is smaller than previous line.
    else if (!ignore_indent &&
             indent.length() < indent_stack_.back() &&
             core != "" &&
             core[0] != '#') {
      // Add as many DEDENT tokens as necessary if indentation matches a previous level.
      if (onStack(indent.length(), indent_stack_)) {
        while (indent_stack_.back() > indent.length()) {
          indent_stack_.pop_back();
          outfile_ << string(kDedent) << " ";
        }
      }
      else {
        cerr << "-- line "
             << line_count
             << ": IndentationError: unindent does not match any outer indentation level\n";
      }
    }

    // Step through each character to identify nested structures and handle
    // implicit/explicit line-joining.
    bool is_empty = true;
    ignore_indent = false;
    string last_chars = "";
    for (const char c : core) {
      // No need to print comments.
      if (c == '#' && !inString())
        break;
      is_empty = false;

      // Introduce nesting.
      if (c == '(' ||
          c == '[' ||
          c == '{' ||
          (c == '\'' && !inString() && last_chars != "\\") ||
          (c == '"' && !inString() && last_chars != "\\" && last_chars != "\"")) {
        if (c == '"' && last_chars == "\"\"") {
          nesting_stack_.push_back("\"\"\"");
        }
        else
          nesting_stack_.push_back(string(1, c));
      }
      // Close nesting.
      else if (c == ')' ||
               c == ']' ||
               c == '}' ||
               (c == '\'' && inString() && last_chars != "\\") ||
               (c == '"' && inString() && last_chars != "\\")) {
        string closing_element = string(1, c);
        if (c == '"' && last_chars == "\"\"")
          closing_element = "\"\"\"";

        if (nesting_stack_.back() == nesting_map_[closing_element])
          nesting_stack_.pop_back();
      }

      // Outside strings, a backslash can only be used for explicit line-joining.
      if (c == '\\' && !inString())
        ignore_indent = true;
      else
        outfile_ << c;

      last_chars = setLastChars(c, last_chars);
    }

    if (implicitJoin())
      ignore_indent = true;

    outfile_ << endl;
    // Newline tokens are inserted on next line, so that comments can run until '\n'.
    if (!is_empty && !ignore_indent)
      outfile_ << string(kNewline) << " ";

    line_count++;
  }

  // Unindents at end of file.
  while (indent_stack_.back() > 0) {
    indent_stack_.pop_back();
    outfile_ << string(kDedent) << " ";
  }
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

// Returns true if the current line causes an implicit line-join,
// i.e. if an opening element is on top of the nesting stack.
bool Preprocessor::implicitJoin() {
  const string s = nesting_stack_.back();
  return (s == "(" ||
          s == "[" ||
          s == "{" ||
          s == "\"\"\"");
}

// Returns true, if the current character is within a string,
// based on the nesting stack.
bool Preprocessor::inString() {
  const string s = nesting_stack_.back();
  return (s == "\'" ||
          s == "\"" ||
          s == "\"\"\"");
}

// Returns a string with the last relevant chars, usually this is a single char.
// Takes the current char <c> and the previous string of last chars.
string Preprocessor::setLastChars(const char &c, const string &last_chars) {
  string result;
  // To distinguish between single- and triple-quoted strings.
  if (c == '"' && last_chars == "\"")
    result = "\"\""; // -> ""
  // To allow escaped quotes before closing a triple-quoted string.
  else if (c == '"' && last_chars == "\\")
    result = "\\\""; // -> \"
  // To make sure an escaped backslash won't escape other chars.
  else if (c == '\\' && last_chars == "\\")
    result = "\\\\"; // -> \\
  // Otherwise only the very last char is relevant.
  else
    result = string(1, c);
  return result;
}

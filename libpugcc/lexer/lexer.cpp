#include <cassert>
#include <iterator>
#include <ostream>
#include <sstream>

#include "lexer.h"

using namespace pug;
using Uint = Lexer::Uint;

namespace ctype {
  bool EndOfLine(char ch) { return ch == '\n'; }
  bool Space(char ch) { return ch == ' '; }
} // namespace ctype

bool
pug::Lexer::Fetch()
{
  if (line_ != 0) { // not the first line
    assert(("input source in valid state", input().isValid()));
    assert(("reached end of line before call", input().isEmpty()));

    input_ = { input_.begin(),
               (Uint)input_.pos(source_.end()) }; // the rest of the source
    if (input().size() <= 1) {
      return false; // end of source
    }
    input().Discard(1); // EndOfLine
  } else {
    // input_ == source_  // constructor
    if (input().isEmpty()) {
      return false; // end of source -- source is empty
    }
  }
  // input_ contains the rest of the source

  input_ = input().ReadUntil(ctype::EndOfLine);
  ++line_;

  Uint indent = TrimSpaces();
  if (indent % indent_size != 0) {
    throw error<InvalidIndentationError>();
  }
  indent_level_ = indent / indent_size;

  if (input().isEmpty()) { // empty line
    return Fetch();
  }

  return true;
}

Uint
pug::Lexer::TrimSpaces()
{
  return input_.DiscardUntilNot(ctype::Space);
}

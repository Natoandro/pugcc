#ifndef _LIBPUGCC_LEXER_LEXER_H_
#define _LIBPUGCC_LEXER_LEXER_H_

#include <algorithm>
#include <iterator>
#include <string_view>

#include "../error.h"
#include "source.h"

namespace pug {

  class Lexer {

  public:
    using Uint = typename Source::Uint;

    struct Position {
      Uint line;
      Uint col;
    };

  public:
    Lexer(Source source, Uint _indent_size = 2):
      source_{ source }, input_{ source }, indent_size{ _indent_size }
    {}

    // fetch the next line into input()
    bool Fetch();

    Uint TrimSpaces();

    Uint indent_level() const { return indent_level_; }

    template <class ErrorClass>
    ErrorClass error()
    {
      return ErrorClass{ *this };
    }

    InputSource& input() { return input_; }
    Source& source() { return source_; }

    Position position() const { return { line_, input_.offset() + 1 }; }

  private:
    InputSource input_{};
    Uint indent_level_;
    Uint line_ = 0;
    std::string filename_{ "Pug" };
    Source source_;

    const Uint indent_size;
  };

  struct InvalidIndentationError : public Error {
    InvalidIndentationError(Lexer& lexer):
      Error("InvalidIndentationError", lexer)
    {}
  };

} // namespace pug

#endif // _LIBPUGCC_LEXER_LEXER_H_

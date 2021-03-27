#ifndef _LIBPUGCC_ERROR_H_
#define _LIBPUGCC_ERROR_H_

#include <stdexcept>

namespace pug {
  class Lexer;

  struct Error : std::runtime_error {
    Error(const char* name, Lexer& lexer):
      std::runtime_error(name), lexer_{ lexer }
    {}

    // TODO: override what()
    // Show current position of the input (from lexer)
    // virtual const char* what() const noexcept override;

  private:
    Lexer& lexer_;
  };
} // namespace pug

#endif // _LIBPUGCC_ERROR_H_

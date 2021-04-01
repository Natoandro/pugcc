#ifndef _LIBPUGCC_OUTPUT_RENDERABLE_H_
#define _LIBPUGCC_OUTPUT_RENDERABLE_H_

#include <cstddef>
#include <memory>
#include <ostream>
#include <type_traits>
#include <vector>

#include <pugcc/io/output_buffer.h>

namespace pug {

  class OutputBuffer;

  // clang-format off

  template <typename T>
  concept Renderable = requires(T& r, OutputBuffer& buf)
  {
    { r.Render() } -> std::convertible_to<std::size_t>;
    { r.Render(buf) };
  };

  // clang-format on

  OutputBuffer& operator<<(OutputBuffer& out, const Renderable auto& r)
  {
    r.Render(out);
    return out;
  }

  std::string to_string(const Renderable auto& r)
  {
    pug::StringBuffer buf{ r.Render() };
    r.Render(buf);
    return std::move(buf);
  }

} // namespace pug

#endif // _LIBPUGCC_OUTPUT_RENDERABLE_H_

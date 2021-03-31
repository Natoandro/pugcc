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

  class Renderable {
  public:
    virtual void Render(OutputBuffer& buf) const = 0;
    virtual std::size_t Render() const = 0;

    explicit operator std::string() const
    {
      pug::StringBuffer buf{ Render() };
      Render(buf);
      return std::move(buf);
    }
  };

  inline OutputBuffer& operator<<(OutputBuffer& out, const Renderable& r)
  {
    r.Render(out);
    return out;
  }

} // namespace pug

#endif // _LIBPUGCC_OUTPUT_RENDERABLE_H_

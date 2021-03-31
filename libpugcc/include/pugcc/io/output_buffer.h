#ifndef _LIBPUGCC_INCLUDE_PUGCC_IO_OUTPUT_BUFFER_H_
#define _LIBPUGCC_INCLUDE_PUGCC_IO_OUTPUT_BUFFER_H_

#include <algorithm>
#include <cstddef>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

namespace pug::outputbuffer_internal {
  using Buffer = std::span<char>;
  using Iter = Buffer::iterator;

  // no range check
  inline Iter Copy(std::string_view src, Iter dest)
  {
    return std::copy_n(src.begin(), src.size(), dest);
  }
} // namespace pug::outputbuffer_internal

namespace pug {

  class OutputBuffer {
  public:
    using Buffer = std::span<char>;
    using Size = Buffer::size_type;
    using Iter = Buffer::iterator;

  public:
    OutputBuffer(Buffer buffer = {}): buffer_{ buffer }
    {
      current_ = buffer_.begin();
    }

    OutputBuffer(const OutputBuffer&) = delete;
    OutputBuffer(OutputBuffer&&) = delete;

    Iter begin() const { return buffer_.begin(); }
    Iter end() const { return current_; }

    Size size() const { return end() - begin(); }

    /* current content of the output buffer */
    operator std::string_view() const { return { buffer_.data(), size() }; }

    void Append(std::string_view buf)
    {
      using namespace outputbuffer_internal;
      if (avail() >= buf.size()) {
        current_ = Copy(buf, current_);
      } else {
        const Size count = avail();
        Copy(buf.substr(0, count), current_);
        buf.remove_prefix(count);
        HandleOverflow();
        Append(buf);
      }
    }

  protected:
    virtual void HandleOverflow()
    {
      throw std::overflow_error{ "OutputBuffer: buffer cannot expand" };
    }

    void set_buffer(Buffer buffer)
    {
      buffer_ = buffer;
      current_ = buffer_.begin();
    }

  private:
    Size avail() const { return buffer_.end() - end(); }

  protected:
    Buffer buffer_;
    Iter current_;
  };

  inline OutputBuffer& operator<<(OutputBuffer& out, std::string_view buf)
  {
    out.Append(buf);
    return out;
  }

  class StringBuffer : public OutputBuffer {
  public:
    StringBuffer(Size capacity): OutputBuffer{}, storage_{}
    {
      storage_.resize(capacity);
      set_buffer(storage_);
    }

    operator std::string() &&
    {
      storage_.resize(size());
      return std::move(storage_);
    }

  private:
    std::string storage_;
  };

  template <typename T>
  StringBuffer& operator<<(StringBuffer& out, T&& t)
  {
    operator<<(static_cast<OutputBuffer&>(out), std::forward<T>(t));
    return out;
  }

} // namespace pug

#endif // _LIBPUGCC_INCLUDE_PUGCC_IO_OUTPUT_BUFFER_H_

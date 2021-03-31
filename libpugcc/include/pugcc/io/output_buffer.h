#ifndef _LIBPUGCC_INCLUDE_PUGCC_IO_OUTPUT_BUFFER_H_
#define _LIBPUGCC_INCLUDE_PUGCC_IO_OUTPUT_BUFFER_H_

#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

namespace pug {

  // TODO: use std::span<char>

  class Buffer {
  public:
    using Size = std::size_t;
    using Iter = char*;
    using ConstIter = const char*;

    Buffer() = default;
    Buffer(char* start, Size size): start_{ start } { finish_ = start_ + size; }
    Buffer(std::string& str): Buffer{ str.data(), str.size() } {}
    Buffer(const Buffer&) = default;
    Buffer(Buffer&&) = delete;

    Buffer& operator=(const Buffer&) = default;
    Buffer& operator=(Buffer&&) = delete;

    size_t size() const { return end() - begin(); }

    Iter begin() { return start_; }
    ConstIter begin() const { return start_; }

    Iter end() { return finish_; }
    ConstIter end() const { return finish_; }

    operator std::string_view() const { return { begin(), size() }; }

    /* no range check */
    void Write(Size offset, std::string_view buf)
    {
      std::copy_n(buf.begin(), buf.size(), begin() + offset);
    }

    /* no range check */
    Iter Write(Iter iter, std::string_view buf)
    {
      return std::copy_n(buf.begin(), buf.size(), iter);
    }

  private:
    char* start_;
    char* finish_;
  };

  class OutputBuffer {
  public:
    using Size = Buffer::Size;
    using Iter = Buffer::Iter;
    using ConstIter = Buffer::ConstIter;

  public:
    OutputBuffer(Buffer buffer = {}): buffer_{ buffer }
    {
      current_ = buffer_.begin();
    }

    OutputBuffer(const OutputBuffer&) = delete;
    OutputBuffer(OutputBuffer&&) = delete;

    Size size() const { return end() - begin(); }
    Size capacity() const { return buffer_.size(); }

    Iter begin() { return buffer_.begin(); }
    ConstIter begin() const { return buffer_.begin(); }

    Iter end() { return current_; }
    ConstIter end() const { return current_; }

    /* current content of the output buffer, which may change upon overflow */
    operator std::string_view() const { return { begin(), size() }; }

    void Append(std::string_view buf)
    {
      if (avail() >= buf.size()) {
        current_ = buffer_.Write(end(), buf);
      } else {
        const Size count = avail();
        buffer_.Write(end(), { buf.begin(), avail() });
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

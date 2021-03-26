#ifndef _LIBPUGCC_LEXER_SOURCE_H_
#define _LIBPUGCC_LEXER_SOURCE_H_

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string_view>

namespace pug {

  class Source {
  public:
    using View = std::string_view;
    using Iter = View::iterator;
    using Uint = View::size_type;

  public:
    template <typename... Args>
    Source(Args&&... args): view_{ std::forward<Args>(args)... }
    {}

    Source(Iter first, Iter last): view_{ first, Distance(first, last) } {}

    bool empty() const { return view_.empty(); }
    Uint getIndex(Iter it) const { return std::distance(view_.begin(), it); }

    /* get first char - no size check */
    char front() const { return view_.front(); }

    template <class Predicate>
    bool frontIs(Predicate pred) const
    {
      return pred(front());
    }

    template <class Predicate>
    Iter find(Predicate pred) const
    {
      return std::find_if(view_.begin(), view_.end(), pred);
    }

    template <class Predicate>
    Iter findNot(Predicate pred) const
    {
      return std::find_if_not(view_.begin(), view_.end(), pred);
    }

    Iter begin() const { return view_.begin(); }
    Iter end() const { return view_.end(); }

    Uint size() const { return view_.size(); }

    operator std::string_view() const { return view_; }
    explicit operator std::string() const { return std::string{ view_ }; }

  private:
    static std::size_t Distance(Iter first, Iter last)
    {
      if (last < first) {
        throw std::range_error("Invalid iterator");
      }
      return (Uint)std::distance(first, last);
    }

  protected:
    std::string_view view_;
  };

  class InputSource : public Source {
  public:
    template <typename... Args>
    InputSource(Args&&... args):
      Source{ std::forward<Args>(args)... }, first_{ view_.begin() }
    {}

    Uint currentIndex() const { return std::distance(first_, view_.begin()); }

    void discard(Uint count = 1) { view_.remove_prefix(count); }

    template <class Predicate>
    Uint discardUntil(Predicate pred)
    {
      return readUntil(pred).size();
    }

    template <class Predicate>
    Uint discardUntilNot(Predicate pred)
    {
      return readUntilNot(pred).size();
    }

    template <class Predicate>
    Source readUntil(Predicate pred)
    {
      Source rv{ begin(), find(pred) };
      discard(rv.size());
      return rv;
    }

    template <class Predicate>
    Source readUntilNot(Predicate pred)
    {
      Source rv{ begin(), findNot(pred) };
      discard(rv.size());
      return rv;
    }

  private:
    Iter first_;
  };

} // namespace pug

#endif // _LIBPUGCC_LEXER_SOURCE_H_

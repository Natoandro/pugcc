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
    using Int = View::difference_type;

  public:
    template <typename... Args>
    Source(Args&&... args): view_{ std::forward<Args>(args)... }
    {}

    Source(Iter first, Iter last): view_{ first, Distance(first, last) } {}

    bool isEmpty() const { return view_.empty(); }

    /* get the index of the item pointed by `it` */
    Int pos(Iter it) const { return std::distance(view_.begin(), it); }

    /* get first char - no size check */
    char front() const { return view_.front(); }

    template <class Predicate>
    bool frontIs(Predicate pred) const
    {
      return pred(front());
    }

    template <class Predicate>
    Iter Find(Predicate pred) const
    {
      return std::find_if(view_.begin(), view_.end(), pred);
    }

    template <class Predicate>
    Iter FindNot(Predicate pred) const
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

    /* get the current offset of the cursor position `begin()` */
    Uint offset() const { return std::distance(first_, view_.begin()); }

    /* Discards `count` characters from the input.

      This does not check the size of the input!!
      The caller must ensure size() <= count before the call so that the input
      remains in a valid state.
     */
    void Discard(Uint count = 1) { view_.remove_prefix(count); }

    template <class Predicate>
    Uint DiscardUntil(Predicate pred)
    {
      return ReadUntil(pred).size();
    }

    template <class Predicate>
    Uint DiscardUntilNot(Predicate pred)
    {
      return ReadUntilNot(pred).size();
    }

    template <class Predicate>
    Source ReadUntil(Predicate pred)
    {
      Source rv{ begin(), Find(pred) };
      Discard(rv.size());
      return rv;
    }

    template <class Predicate>
    Source ReadUntilNot(Predicate pred)
    {
      Source rv{ begin(), FindNot(pred) };
      Discard(rv.size());
      return rv;
    }

    bool isValid() const { return begin() <= end(); }

  private:
    Iter first_;
  };

} // namespace pug

#endif // _LIBPUGCC_LEXER_SOURCE_H_

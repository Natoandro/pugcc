#ifndef _LIBPUGCC_UTILS_ALGO_H_
#define _LIBPUGCC_UTILS_ALGO_H_

#include <algorithm>

namespace pug::utils {

  template <class Container, class Predicate>
  auto Find(Container& c, Predicate pred)
  {
    return std::find_if(c.begin(), c.end(), pred);
  }

} // namespace pug::utils

#endif // _LIBPUGCC_UTILS_ALGO_H_

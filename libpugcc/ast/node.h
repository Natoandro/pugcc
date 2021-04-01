#ifndef _LIBPUGCC_AST_NODE_H_
#define _LIBPUGCC_AST_NODE_H_

#include <cstddef>
#include <memory>
#include <numeric>
#include <ostream>
#include <type_traits>
#include <utility>
#include <vector>

#include "../output/renderable.h"

namespace pug {

  class Node {
  public:
    virtual void Render(OutputBuffer& buf) const = 0;
    virtual std::size_t Render() const = 0;
  };
  static_assert(Renderable<Node>);

  namespace childlist_internal {
    inline std::size_t size(const std::unique_ptr<Node>& p_node)
    {
      return p_node->Render();
    }
  } // namespace childlist_internal

  class NodeList : public std::vector<std::unique_ptr<Node>> {
  public:
    void Render(OutputBuffer& out) const
    {
      for (auto& p_node : *this) {
        out << *p_node;
      }
    }

    std::size_t Render() const
    {
      using childlist_internal::size;
      return std::transform_reduce(begin(), end(), 0, std::plus<>(), size);
    }
  };
  static_assert(Renderable<NodeList>);

  // TODO: use concept
  template <typename T>
  constexpr bool isNode = std::is_base_of_v<Node, std::decay_t<T>>;

  class ParentNode {
  public:
    template <class Child, typename... Args>
    std::enable_if_t<isNode<Child>, Child>& AppendChild(
      std::in_place_t,
      Args&&... args)
    {
      return AppendChild(std::make_unique<Child>(std::forward<Args>(args)...));
    }

    template <class Child>
    std::enable_if_t<isNode<Child>, Child>& AppendChild(
      std::unique_ptr<Child>&& ptr)
    {
      children_.push_back(std::move(ptr));
      return static_cast<Child&>(*children_.back());
    }

  protected:
    NodeList& children() { return children_; }
    const NodeList& children() const { return children_; }

  private:
    NodeList children_;
  };

} // namespace pug

#endif // _LIBPUGCC_AST_NODE_H_

#ifndef _LIBPUGCC_AST_NODE_H_
#define _LIBPUGCC_AST_NODE_H_

#include <concepts>
#include <cstddef>
#include <memory>
#include <numeric>
#include <ostream>
#include <type_traits>
#include <utility>
#include <vector>

#include "../output/renderable.h"

namespace pug {

  /** Base class of all nodes */
  class Node {
  public:
    virtual void Render(OutputBuffer& buf) const = 0;
    virtual std::size_t Render() const = 0;
  };
  static_assert(Renderable<Node>);

} // namespace pug

namespace pug::nodelist_internal {
  inline std::size_t size(const std::unique_ptr<Node>& p_node)
  {
    return p_node->Render();
  }
} // namespace pug::nodelist_internal

namespace pug {
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
      using nodelist_internal::size;
      return std::transform_reduce(begin(), end(), 0, std::plus<>(), size);
    }
  };
  static_assert(Renderable<NodeList>);

  /** Nodes that (can) have children */
  class ParentNode {
  public:
    template <std::derived_from<Node> Child, typename... Args>
    Child& AppendChild(std::in_place_t, Args&&... args)
    {
      return AppendChild(std::make_unique<Child>(std::forward<Args>(args)...));
    }

    template <std::derived_from<Node> Child>
    Child& AppendChild(std::unique_ptr<Child>&& ptr)
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

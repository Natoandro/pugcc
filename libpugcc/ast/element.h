#ifndef _LIBPUGCC_AST_ELEMENT_H_
#define _LIBPUGCC_AST_ELEMENT_H_

#include <map>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../output/renderable.h"
#include "node.h"

namespace pug {

  // Currently, attribute value is a string, future versions will use
  // pug::Expression

  /** Attribute list of an element */
  class AttrList : public std::map<std::string, std::string> {
  public:
    void Render(OutputBuffer& buf) const;
    std::size_t Render() const;
  };
  static_assert(Renderable<AttrList>);

  /** Class list
   * Classes are not included in AttrList.
   */
  class ClassList : public std::set<std::string> {
  public:
    void Render(OutputBuffer& buf) const;
    std::size_t Render() const;
  };
  static_assert(Renderable<ClassList>);

  class ParentElement;

  /**
   * Instances of this class (non-derived) match to elements that have no
   * closing tag.
   */
  class Element : public Node {

  public:
    Element(std::string_view tag_name): tag_name_{ tag_name } {}

    void Render(OutputBuffer& out) const override;
    std::size_t Render() const override;

    void Set(std::string attr_name, std::string attr_value)
    {
      if (attr_name == "class") class_list_.insert(std::move(attr_value));
      else
        attrs_.insert_or_assign(std::move(attr_name), std::move(attr_value));
    }

    ClassList& class_list() { return class_list_; }

  private:
    std::string tag_name_;
    AttrList attrs_;
    ClassList class_list_;
  };

  /**
   * Instances match to elements that have closing tag, and thus can have child
   * nodes.
   */
  class ParentElement : public Element, public ParentNode {
    friend class Element; // for Render function

  public:
    using Element::Element;
  };

} // namespace pug

#endif // _LIBPUGCC_AST_ELEMENT_H_

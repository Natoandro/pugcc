#include "element.h"

#include <pugcc/io/output_buffer.h>

using namespace pug;

void
AttrList::Render(OutputBuffer& out) const
{
  for (auto& entry : *this) {
    out << " " << entry.first << "=\"" << entry.second << "\"";
  }
}

std::size_t
AttrList::Render() const
{
  // for each entry: leading space, equal sign, 2 quotes
  std::size_t length = size() * 4;
  for (auto& entry : *this) {
    length += entry.first.size() + entry.second.size();
  }
  return length;
}

void
ClassList::Render(OutputBuffer& out) const
{
  using namespace std::string_view_literals;
  if (empty()) return;
  auto it = begin();
  out << *it;
  while (++it != end()) {
    out << " "sv << *it;
  }
}

std::size_t
ClassList::Render() const
{
  if (empty()) return 0;
  auto it = begin();
  std::size_t res = it->size();
  while (++it != end()) res += 1 + it->size();
  return res;
}

void
Element::Render(OutputBuffer& out) const
{
  out << "<" << tag_name_;
  if (!class_list_.empty())
    out << " class=\"" << static_cast<std::string>(class_list_) << "\"";
  out << attrs_ << ">";

  auto* ptr = dynamic_cast<const ParentElement*>(this);

  // no closing tag
  if (ptr == nullptr) return;
  out << ptr->children();
  out << "</" << tag_name_ << ">";
}

std::size_t
Element::Render() const
{
  std::size_t rv = 2 + tag_name_.size() + attrs_.Render();
  if (!class_list_.empty()) rv += 9 + class_list_.Render();

  // closing tag
  auto* ptr = dynamic_cast<const ParentElement*>(this);
  if (ptr == nullptr) return rv;
  for (auto& p_child : ptr->children()) {
    rv += p_child->Render();
  }
  rv += 3 + tag_name_.size();
  return rv;
}

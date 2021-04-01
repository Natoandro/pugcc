#include "../../libpugcc/ast/element.h"
#include <pugcc/io/output_buffer.h>

#include <gtest/gtest.h>

using namespace pug;

class AttrListTest : public ::testing::Test {
protected:
  void SetUp() override
  {
    list1.insert_or_assign("name", "element");

    list2.insert_or_assign("id", "ActionButton");
    list2.insert_or_assign("title", "Button action...");
  }

  AttrList list0;
  AttrList list1;
  AttrList list2;
};

TEST_F(AttrListTest, AttrList)
{
  EXPECT_EQ("", to_string(list0));

  EXPECT_EQ(" name=\"element\"", to_string(list1));

  EXPECT_EQ(
    " id=\"ActionButton\" title=\"Button action...\"",
    to_string(list2));
}

class ClassListTest : public ::testing::Test {

protected:
  void SetUp() override
  {
    cl1.insert("foo");

    cl2.insert("bar");
    cl2.insert("baz");

    cl3 = cl2;
    cl3.insert("foooo");
  }

  ClassList cl0;
  ClassList cl1;
  ClassList cl2;
  ClassList cl3;
};

TEST_F(ClassListTest, ClassList)
{
  EXPECT_EQ("", to_string(cl0));

  EXPECT_EQ(3, cl1.Render());
  EXPECT_EQ("foo", to_string(cl1));

  // ClassList current implementation is std::set -- ordered set
  EXPECT_EQ("bar baz", to_string(cl2));

  cl2.insert("bar");
  EXPECT_EQ(7, cl2.Render());

  EXPECT_EQ("bar baz foooo", to_string(cl3));
}

class ElementTest : public ::testing::Test {
protected:
  void SetUp() override
  {
    el1.Set("rel", "stylesheet");
    el1.Set("href", "/static/style.css");

    el2.Set("src", "/images/image001.jpg");
    el2.class_list().insert("image");
    el2.class_list().insert("button-image");
  }

  Element el0{ "br" };
  Element el1{ "link" };
  Element el2{ "img" };
};

TEST_F(ElementTest, Element)
{
  EXPECT_EQ("<br>", to_string(el0));

  EXPECT_EQ(
    "<link href=\"/static/style.css\" rel=\"stylesheet\">",
    to_string(el1));
}

TEST_F(ElementTest, ElementWithClass)
{
  EXPECT_EQ(
    "<img class=\"button-image image\" src=\"/images/image001.jpg\">",
    to_string(el2));
}

class ParentElementTest : public ::testing::Test {
protected:
  void SetUp() override
  {
    p1.Set("href", "https://localhost:3210/");
    auto& img = p1.AppendChild(std::make_unique<Element>("img"));
    img.Set("src", "/images/mouse.jpg");

    p2.Set("id", "member-list");
    auto& mb1 = p2.AppendChild(std::make_unique<ParentElement>("li"));
    mb1.Set("id", "mb1");
    auto& mb2 = p2.AppendChild(std::make_unique<ParentElement>("li"));
    mb2.Set("id", "mb2");
  }

  ParentElement p0{ "div" };
  ParentElement p1{ "a" };
  ParentElement p2{ "ul" };
};

TEST_F(ParentElementTest, WithoutChild)
{
  EXPECT_EQ("<div></div>", to_string(p0));
}

TEST_F(ParentElementTest, WithOneChild)
{
  EXPECT_EQ(
    "<a href=\"https://localhost:3210/\"><img src=\"/images/mouse.jpg\"></a>",
    to_string(p1));
}

TEST_F(ParentElementTest, WithChildren)
{
  EXPECT_EQ(
    "<ul id=\"member-list\"><li id=\"mb1\"></li><li id=\"mb2\"></li></ul>",
    to_string(p2));
}

#include "../../libpugcc/lexer/source.h"

#include "gtest/gtest.h"

#include <cctype>
#include <string>

TEST(Source, EmptySource)
{
  std::string str{};
  pug::Source source{ str };
  EXPECT_TRUE(source.empty());
}

TEST(Source, NonEmptySource)
{
  auto Space = [](char ch) { return ch == ' '; };
  auto Alpha = [](char ch) { return std::isalpha(ch); };

  pug::Source src{ "test string" };

  EXPECT_FALSE(src.empty());
  EXPECT_EQ(src.size(), 11);
  EXPECT_EQ(src.getIndex(src.find(Space)), 4);
  EXPECT_TRUE(src.frontIs(Alpha));
  EXPECT_EQ(src.getIndex(src.find(Alpha)), 0);
  EXPECT_EQ(src.getIndex(src.findNot(Alpha)), 4);
}

TEST(InputSource, ReadingFromInputSource)
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
  auto Space = [](char ch) { return ch == ' '; };
  auto Alpha = [](char ch) { return std::isalpha(ch); };

  pug::InputSource src{ "this is a test input" };
  EXPECT_EQ(src.readUntil(Space), "this"sv);
  src.discard(1);
  EXPECT_EQ(src.currentIndex(), 5);
  src.discardUntilNot(Alpha);
  EXPECT_TRUE(src.frontIs(Space));
  src.discardUntil(Alpha);
  EXPECT_EQ((std::string)src, "a test input"s);
}

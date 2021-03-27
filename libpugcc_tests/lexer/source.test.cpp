#include "../../libpugcc/lexer/source.h"

#include "gtest/gtest.h"

#include <cctype>
#include <string>

TEST(Source, EmptySource)
{
  std::string str{};
  pug::Source source{ str };
  EXPECT_TRUE(source.isEmpty());
}

TEST(Source, NonEmptySource)
{
  auto Space = [](char ch) { return ch == ' '; };
  auto Alpha = [](char ch) { return std::isalpha(ch); };

  pug::Source src{ "test string" };

  EXPECT_FALSE(src.isEmpty());
  EXPECT_EQ(src.size(), 11);
  EXPECT_EQ(src.pos(src.Find(Space)), 4);
  EXPECT_TRUE(src.frontIs(Alpha));
  EXPECT_EQ(src.pos(src.Find(Alpha)), 0);
  EXPECT_EQ(src.pos(src.FindNot(Alpha)), 4);
}

TEST(InputSource, ReadingFromInputSource)
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
  auto Space = [](char ch) { return ch == ' '; };
  auto Alpha = [](char ch) { return std::isalpha(ch); };

  pug::InputSource input{ "this is a test input" };
  EXPECT_EQ(input.ReadUntil(Space), "this"sv);
  input.Discard(1);
  EXPECT_EQ(input.offset(), 5);
  input.DiscardUntilNot(Alpha);
  EXPECT_TRUE(input.frontIs(Space));
  input.DiscardUntil(Alpha);
  EXPECT_EQ((std::string)input, "a test input"s);
}

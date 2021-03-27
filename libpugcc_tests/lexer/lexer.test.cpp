#include "../../libpugcc/lexer/lexer.h"

#include <cctype>
#include <gtest/gtest.h>

class LexerTest : public ::testing::Test {
protected:
  using Lexer = pug::Lexer;
  using Source = pug::Source;

  LexerTest():
    empty_{},
    no_indent_("source without indentation"),
    invalid_indent_{ "   invalid indentation" },
    level2_indent_{ "    with level-two indentation" },
    empty_first_line_{ "\n    empty first line" },
    empty_first_lines_{ "\n  \nafter two empty lines" },
    empty_lines_{ "\n  \n    \n  " }
  {}

  Source empty_;
  Source no_indent_;
  Source invalid_indent_;
  Source level2_indent_;
  Source empty_first_line_;
  Source empty_first_lines_;
  Source empty_lines_;

  // ctypes
  static bool Alpha(char ch) { return std::isalpha(ch); }
  static bool Space(char ch) { return ch == ' '; }
};

TEST_F(LexerTest, EmptySource)
{
  Lexer lexer{ empty_ };
  EXPECT_FALSE(lexer.Fetch());
}

TEST_F(LexerTest, NoIndent)
{
  Lexer lexer{ no_indent_ };

  EXPECT_TRUE(lexer.Fetch());
  EXPECT_EQ(lexer.indent_level(), 0);
  lexer.input().Discard(lexer.source().size());
  EXPECT_FALSE(lexer.Fetch());
}

TEST_F(LexerTest, SkippingNonEmptyLine)
{
  Lexer lexer = { no_indent_, 2 };
  lexer.Fetch();
  ASSERT_DEATH(lexer.Fetch(), "");
}

TEST_F(LexerTest, InvalidIndentation)
{
  Lexer lexer{ invalid_indent_ };
  ASSERT_THROW(lexer.Fetch(), pug::InvalidIndentationError);
}

TEST_F(LexerTest, ReadingIndentation)
{
  Lexer lexer{ level2_indent_ };
  EXPECT_TRUE(lexer.Fetch());
  EXPECT_EQ(lexer.indent_level(), 2);
  EXPECT_FALSE(lexer.input().frontIs(Space));
  EXPECT_TRUE(lexer.input().frontIs(Alpha));
}

TEST_F(LexerTest, EmptyLines)
{
  Lexer lexer1{ empty_first_line_ };
  Lexer lexer2{ empty_first_lines_ };

  EXPECT_TRUE(lexer1.Fetch());
  EXPECT_EQ(lexer1.position().line, 2);
  EXPECT_EQ(lexer1.indent_level(), 2);

  EXPECT_TRUE(lexer2.Fetch());
  EXPECT_EQ(lexer2.position().line, 3);
}

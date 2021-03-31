#include <pugcc/io/output_buffer.h>

#include <gtest/gtest.h>
#include <stdexcept>
#include <string_view>

using namespace pug;

class StringBufferTest : public ::testing::Test {
protected:
  void SetUp() override
  {
    sb1 << sv1;
    sb2 << sv1 << sv2;
  }

  std::string_view sv1{ "a test string" };
  std::string_view sv2{ ", and a second one..." };
  StringBuffer sb0{ 0 };
  StringBuffer sb1{ 14 };
  StringBuffer sb2{ 36 };
  StringBuffer sb3{ 63 };
};

TEST_F(StringBufferTest, Overflow)
{
  EXPECT_THROW(sb0 << "a", std::overflow_error);
  EXPECT_THROW(sb1 << sv1, std::overflow_error);
}

TEST_F(StringBufferTest, Append)
{
  EXPECT_EQ(sv1, std::string{ sb1 });

  std::string s{ sv1 };
  s.append(sv2);
  EXPECT_EQ(s, std::string{ sb2 });

  sb3 << sv1 << sv1 << sv2;
  EXPECT_EQ(
    "a test stringa test string, and a second one...",
    std::string{ sb3 });
}

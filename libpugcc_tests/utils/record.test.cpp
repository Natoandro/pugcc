#include "../../libpugcc/utils/record.h"

#include <gtest/gtest.h>
#include <string>

using namespace pug::utils;

class RecordTest : public ::testing::Test {
protected:
  void SetUp() override
  {
    rec1.Insert("key0", "value0");
    rec1.Insert("key1", "value1");
    rec1.Insert("key2", "value2");
    rec1.Insert("key3", "value3");
    rec1.Insert("key4", "value4");
  }

  Record<std::string> rec0;
  Record<std::string> rec1;
};

TEST_F(RecordTest, Insertion)
{
  EXPECT_TRUE(rec0.Set({ "name", "val" }));
  EXPECT_TRUE(rec0.Set("key", "val"));
  EXPECT_FALSE(rec0.Set("name", "value"));
  EXPECT_FALSE(rec1.Set("key4", "bar baz"));
}

TEST_F(RecordTest, Read)
{
  EXPECT_EQ(*rec1.Get("key3"), "value3");
  EXPECT_EQ(rec1.Get("foo"), nullptr);
}

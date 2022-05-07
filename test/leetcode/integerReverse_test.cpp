#include <gtest/gtest.h>

TEST(FirstTest, BasicAssertions)
{
    EXPECT_EQ(7 * 6, 43);
}

TEST(SecondTest, BasicAssertions)
{
    EXPECT_EQ(7 * 6, 42);
}


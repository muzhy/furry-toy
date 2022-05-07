#include <gtest/gtest.h>

#include "Number.hpp"

TEST(NumberReverseTest, NormalNumber)
{
    EXPECT_EQ(321, leetCode::reverse(123));
}

TEST(NumberReverseTest, NegativeNumber)
{
    EXPECT_EQ(-321, leetCode::reverse(-123));
}

TEST(NumberReverseTest, ContentZeroNumber)
{
    EXPECT_EQ(21, leetCode::reverse(120));
}

TEST(NumberReverseTest, ZeroNumber)
{
    EXPECT_EQ(0, leetCode::reverse(0));
}

TEST(NumberReverseTest, PositiveOverflow)
{
    EXPECT_EQ(0, leetCode::reverse(1534236469));
}

TEST(NumberReverseTest, NegativeOverflow)
{
    EXPECT_EQ(0, leetCode::reverse(-2147483648));
}
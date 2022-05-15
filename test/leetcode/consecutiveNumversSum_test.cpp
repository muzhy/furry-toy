#include <gtest/gtest.h>

#include "Number.hpp"

TEST(ConsecutiveSumTest, One)
{
    EXPECT_EQ(1, leetCode::consecutiveNumbersSum(1));
}

TEST(ConsecutiveSumTest, Five)
{
    EXPECT_EQ(2, leetCode::consecutiveNumbersSum(5));
}

TEST(ConsecutiveSumTest, Nine)
{
    EXPECT_EQ(3, leetCode::consecutiveNumbersSum(9));
}

TEST(ConsecutiveSumTest, LargeNumber)
{
    EXPECT_EQ(4, leetCode::consecutiveNumbersSum(855877922));
}
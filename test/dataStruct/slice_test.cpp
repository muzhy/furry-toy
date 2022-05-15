#include <gtest/gtest.h>

#include "slice.hpp"

using namespace furry_toy;

TEST(SliceTest, CreateEmptySlice)
{
    slice<int> s;
    EXPECT_EQ(s.isEmpty(), true);
    // EXPECT_EQ(s.len(), 0);
    // EXPECT_EQ(s.cap(), 0);
    // EXPECT_EQ(s.toString(), "[]");
}

TEST(SliceTest, InitializeListIinit)
{
    slice<int> s{1, 2, 3};
    EXPECT_EQ(s.len(), 3);
    // for(size_t i = 0; i < 3; i++)
    // {
    //     EXPECT_EQ(s[i], i + 1);
    // }
    // EXPECT_EQ(s.isEmpty(), false);
}
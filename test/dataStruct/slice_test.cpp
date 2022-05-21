#include <gtest/gtest.h>

#include "slice.hpp"

using namespace furry_toy;

class SliceTest : public ::testing::Test
{
protected:
    void SetUp() override{

    }

    slice<int> emptySlice;
    slice<int> initLenValSlice = slice<int>(5, 0);
};

TEST_F(SliceTest, EmptySliceEmptyTest)
{
    EXPECT_EQ(emptySlice.isEmpty(), true);
} 

TEST_F(SliceTest, InitLenValSliceEmptyTest)
{
    EXPECT_EQ(initLenValSlice.isEmpty(), false);
}

TEST_F(SliceTest, EmptySliceLenTest)
{
    EXPECT_EQ(emptySlice.len(), 0);
}

TEST_F(SliceTest, InitLenValSliceLenTest)
{
    EXPECT_EQ(initLenValSlice.len(), 5);
}

TEST_F(SliceTest, EmptySliceCapTest)
{
    EXPECT_EQ(emptySlice.cap(), 0);
}

TEST_F(SliceTest, EmptySliceStringTest)
{
    EXPECT_EQ(emptySlice.toString(), "[]");
}

TEST_F(SliceTest, InitLenValSliceStringTest)
{
    EXPECT_EQ(initLenValSlice.toString(), "[0, 0, 0, 0, 0]");
}

TEST_F(SliceTest, InitLenValSliceOutStreamTest)
{
    std::stringstream ss;
    ss << initLenValSlice;
    EXPECT_EQ(ss.str(), "[0, 0, 0, 0, 0]");
}

TEST_F(SliceTest, EndAppendTest)
{
    auto s2 = initLenValSlice.append({1});
    EXPECT_EQ("[0, 0, 0, 0, 0, 1]", s2.toString());
}

TEST_F(SliceTest, EndAppendPreTest)
{
    initLenValSlice.append({1});
    EXPECT_EQ("[0, 0, 0, 0, 0, 1]", initLenValSlice.toString());
}

TEST_F(SliceTest, AppendFunPreTest)
{
    append(initLenValSlice, {1});
    EXPECT_EQ("[0, 0, 0, 0, 0]", initLenValSlice.toString());
}

TEST_F(SliceTest, AppendFunAfterTest)
{
    auto s1 = append(initLenValSlice, {1});
    EXPECT_EQ("[0, 0, 0, 0, 0, 1]", s1.toString());
}
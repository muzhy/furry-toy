#include <gtest/gtest.h>

#include "Number.hpp"
#include "WinmineSolution.hpp"

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

TEST(WinmineTest, Normal)
{
    using namespace leetCode;
    WinmineSolution::BoardType board = 
    {
        {'E','E','E','E','E'},
        {'E','E','M','E','E'},
        {'E','E','E','E','E'},
        {'E','E','E','E','E'}
    };
    WinmineSolution::BoardPosType clickPos = {3, 0};

    WinmineSolution winmine(board);
    winmine.click(clickPos);    

    WinmineSolution::BoardType resBoard = winmine.getBoard();
    
    WinmineSolution::BoardType expectBoard = {
        {'B','1','E','1','B'},
        {'B','1','M','1','B'},
        {'B','1','1','1','B'},
        {'B','B','B','B','B'}
    };
    
    EXPECT_EQ(resBoard, expectBoard);
}
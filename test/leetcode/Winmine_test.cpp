#include <gtest/gtest.h>

#include "WinmineSolution.hpp"

TEST(WinmineTest, ClickEmpty)
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

TEST(WinmineTest, ClickBoom)
{
    using namespace leetCode;

    WinmineSolution::BoardType board = 
    {
        {'E','E','E','E','E'},
        {'E','E','M','E','E'},
        {'E','E','E','E','E'},
        {'E','E','E','E','E'}
    };
    WinmineSolution::BoardPosType clickPos = {1, 2};

    WinmineSolution winmine(board);
    bool res = winmine.click(clickPos);    

    WinmineSolution::BoardType resBoard = winmine.getBoard();
    
    WinmineSolution::BoardType expectBoard = {
        {'E','E','E','E','E'},
        {'E','E','X','E','E'},
        {'E','E','E','E','E'},
        {'E','E','E','E','E'}
    };
    
    EXPECT_EQ(resBoard, expectBoard);
    EXPECT_FALSE(res);
}

TEST(WinmineTest, ClickNearBoom)
{
    using namespace leetCode;

    WinmineSolution::BoardType board = 
    {
        {'E','E','E','E','E'},
        {'E','E','M','E','E'},
        {'E','E','E','E','E'},
        {'E','E','E','E','E'}
    };
    WinmineSolution::BoardPosType clickPos = {0, 2};

    WinmineSolution winmine(board);
    bool res = winmine.click(clickPos);    

    WinmineSolution::BoardType resBoard = winmine.getBoard();
    
    WinmineSolution::BoardType expectBoard = {
        {'E','E','1','E','E'},
        {'E','E','M','E','E'},
        {'E','E','E','E','E'},
        {'E','E','E','E','E'}
    };
    
    EXPECT_EQ(resBoard, expectBoard);
    EXPECT_TRUE(res);
}
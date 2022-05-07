#include "Solution.hpp"

#include <vector>

namespace leetCode
{
    class WinmineSolution 
    {
    public:
        typedef std::vector<std::vector<char>> BoardType;
        typedef std::vector<int> BoardPosType;

        WinmineSolution(const BoardType& board) : m_board(board){}        
        virtual ~WinmineSolution(){}

        /******************************************************
         * 点击棋盘上的某个位置
         * ****************************************************/
        bool click(const BoardPosType pos);   

        inline BoardType getBoard()
        {
            return m_board;
        }

        /******************************************************
         * 将棋盘输出到控制台上
         * ****************************************************/
        void printBoard();

    private:   
        void updateBoard(const BoardPosType& pos);     
        int findNearBoomCount(const BoardPosType& pos);
        /***********************************************************
         * 解开棋盘中的某个格
         * 若该格不是未挖出的空白格，则不揭开，直接返回
         * 若该格是未挖出的空白格，且周围有炸弹，则揭开该格，并显示周围的炸弹数目
         * 若该格是未挖出的空白格，且周围没有炸弹，则揭开该格，并递归揭开周围的格子
         * **********************************************************/
        void uncover(const BoardPosType& pos);
    private:
        static const int m_sDir[8][2];

        BoardType m_board;
    };
} // namespace leetCode

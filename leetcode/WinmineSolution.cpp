#include "WinmineSolution.hpp"

#include <iostream>

namespace leetCode
{
    using namespace std;

    const int WinmineSolution::m_sDir[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1}, 
        {0, -1}, {0, 1}, 
        {1, -1}, {1, 0}, {1, 1}};

    void WinmineSolution::printBoard()
    {
        for (size_t i = 0; i < m_board.size(); i++)
        {
            for (size_t j = 0; j < m_board[i].size(); j++)
            {
                cout << m_board[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    bool WinmineSolution::click(const BoardPosType pos)
    {
        updateBoard(pos);
        return m_board[pos[0]][pos[1]] != 'X';
    }

    void WinmineSolution::updateBoard(const vector<int> &click)
    {
        switch (m_board[click[0]][click[1]])
        {
        case 'M':
            m_board[click[0]][click[1]] = 'X';
            break;
        case 'E':
            uncover(click);
            break;
        default:
            break;
        }
    }

    void WinmineSolution::uncover(const BoardPosType& pos)
    {
        if(m_board[pos[0]][pos[1]] != 'E')
        {
            return ;
        }
        int nearBoomCount = findNearBoomCount(pos);
        if(nearBoomCount == 0)
        {
            m_board[pos[0]][pos[1]] = 'B';
            // printBoard();
            for (size_t i = 0; i < 8; i++)
            {
                int row = pos[0] + m_sDir[i][0];
                int col = pos[1] + m_sDir[i][1];
                if (row < 0 || row >= m_board.size() || 
                    col < 0 || col >= m_board[row].size())
                {
                    // 超出棋盘
                    continue;
                }
                BoardPosType newPos = {row, col};
                uncover(newPos);
                // printBoard();
            }
        }
        else
        {
            m_board[pos[0]][pos[1]] = nearBoomCount + '0';
        }
    }

    int WinmineSolution::findNearBoomCount(const BoardPosType &pos)
    {
        int nearBoomCount = 0;

        for (size_t i = 0; i < 8; i++)
        {
            int row = pos[0] + m_sDir[i][0];
            int col = pos[1] + m_sDir[i][1];
            if (row < 0 || row >= m_board.size() || 
                col < 0 || col >= m_board[row].size())
            {
                // 超出棋盘
                continue;
            }
            if(m_board[row][col] == 'M')
            {
                nearBoomCount++;
            }
        }

        return nearBoomCount;
    }

} // namespace leetCode

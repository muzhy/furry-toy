#include "WinmineSolution.hpp"

#include <iostream>

namespace leetCode
{
    using namespace std;    

    void WinmineSolution::printBoard(const vector<vector<char>>& board)
    {
        for(size_t i = 0; i < board.size(); i++)
        {
            cout << "[";
            for(size_t j = 0; j < board[i].size(); j++)
            {
                cout << board[i][j] << ", ";
            }
            cout << "]" << endl;
        }
    }

    vector<vector<char>> WinmineSolution::updateBoard(
        vector<vector<char>> &board, vector<int> &click)
    {
        switch (board[click[0]][click[1]])
        {
        case 'M':
            board[click[0]][click[1]] = 'X';
            break;
        case 'E':
            handleSafeClick(board, click);
            break;
        default:
            break;
        }
        return board;
    }

    int WinmineSolution::findBoomCount(
        const std::vector<std::vector<char>>& board,
        const std::vector<int>& click)
    {
        int boomCount = 0;
        for(int i = click[0] - 1; i <= click[0] + 1; i++)
        {
            if(i < 0 || i >= board.size())
            {
                continue;
            }
            for(int j = click[1] - 1; j <= click[1] + 1; j++)
            {
                if(j < 0 || j >= board[i].size())
                {
                    continue;
                }
                if(i == click[0] && j == click[0])
                {
                    continue;
                }

                if(board[i][j] == 'M')
                {
                    boomCount++;
                }
            }
        }
        return boomCount;
    }

    void WinmineSolution::handleSafeClick(std::vector<std::vector<char>>& board,
        const std::vector<int>& click)
    {
        if(board[click[0]][click[1]] != 'E')
        {
            return ;
        }
        int boomCount = findBoomCount(board, click);
        // 周围没有地雷
        if(boomCount == 0)
        {
            board[click[0]][click[1]] = 'B';
 
            for(int i = click[0] - 1; i <= click[0] + 1; i++)
            {
                if(i < 0 || i >= board.size())
                {
                    continue;
                }
                for(int j = click[1] - 1; j <= click[1] + 1; j++)
                {
                    if(j < 0 || j >= board[i].size())
                    {
                        continue;
                    }
                    if(i == click[0] && j == click[0])
                    {
                        continue;
                    }
                    vector<int> pos;
                    pos.push_back(i);
                    pos.push_back(j);
                    int nearBoomCount = findBoomCount(board, pos);
                    if(nearBoomCount == 0)
                    {
                        // 递归暴露周围的地块 
                        handleSafeClick(board, pos);
                    }
                    else
                    {
                        board[i][j] = '0' + boomCount;
                    }
                }
            }          
        }
        else
        {
            board[click[0]][click[1]] = '0' + boomCount;
        }
    }
    
} // namespace leetCode

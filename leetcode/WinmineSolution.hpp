#include "Solution.hpp"

#include <vector>

namespace leetCode
{
    class WinmineSolution : public Solution
    {
    public:
        WinmineSolution(){}        
        virtual ~WinmineSolution(){}

        virtual std::string run();
    private:
        std::vector<std::vector<char>> updateBoard(
            std::vector<std::vector<char>>& board, 
            std::vector<int>& click);
        
        void printBoard(const std::vector<std::vector<char>>& board);
        void handleSafeClick(std::vector<std::vector<char>>& board,
            const std::vector<int>& click);
        int findBoomCount(const std::vector<std::vector<char>>& board,
            const std::vector<int>& click);
    };
} // namespace leetCode

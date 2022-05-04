#include "Solution.hpp"

#include <vector>
#include <map>

namespace leetCode
{   
    /***********************************************************************
     * 给定一个 32 位的有符号整数 x ，返回将 x 中的数字部分反转后的结果。
     * 如果反转后整数超过 32 位的有符号整数的范围 [−2^31,  2^31 − 1] ，就返回 0
     ********************************************************************/
    class IntegerReverseSolution : public Solution
    {
    public:
        IntegerReverseSolution();
        virtual ~IntegerReverseSolution();
        virtual std::string run();
    private:
        int integerReverse(int x);
    private:
        // 测试用例
        std::vector<std::pair<int, int>> testCase;
    };

} // namespace leetCode

#ifndef __LEET_CODE_SOLUTION_H__
#define __LEET_CODE_SOLUTION_H__

#include <string>

namespace leetCode
{
    /********************************************************
     * Solution 参照LeetCode中每道题所给的解决方案的类所定义的
     * 所有的LeetCode的解法的抽象基类。
     * *****************************************************/
    class Solution
    {
    public:
        Solution() {
            
        }
        virtual ~Solution(){

        }
        
        /*****************************************************
         * 调用Solution中的所有测试用例执行，若存在测试用例无法
         * 无法通过，则返回该错误的测试用例的结果。若所有的测试
         * 用例均测试通过，则返回空字符串
         * **************************************************/
        virtual std::string run() = 0;
    };
} // namespace leetCode

#endif
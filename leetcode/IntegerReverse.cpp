#include "IntegerReverse.hpp"

#include <climits>
#include <string>
#include <sstream>

namespace leetCode
{
    using namespace std;
    
    IntegerReverseSolution::IntegerReverseSolution() : Solution()
    {
        testCase.push_back(make_pair(123, 321));
        testCase.push_back(make_pair(-123, -321));
        testCase.push_back(make_pair(120, 21));
        testCase.push_back(make_pair(0, 0));
        testCase.push_back(make_pair(1534236469, 0));
        testCase.push_back(make_pair(-2147483648, 0));
    }

    IntegerReverseSolution::~IntegerReverseSolution()
    {

    }


    int IntegerReverseSolution::integerReverse(int x)
    {
        int res = 0;
        while(x != 0)
        {
             if (res < INT_MIN / 10 || res > INT_MAX / 10) {
                return 0;
            }
            res = res * 10 + x % 10;
            x /= 10;            
        }
        
        return  res;
    }

    std::string IntegerReverseSolution::run()
    {
        std::string res;
        for(size_t i = 0; i < testCase.size(); i++)
        {
            int reverseValue = integerReverse(testCase[i].first);
            if(reverseValue != testCase[i].second)
            {
                stringstream ss;
                ss << "IntegerReverseSolution 执行出错。输入："
                    << testCase[i].first << " 期望：" << testCase[i].second
                    << "结果: " << reverseValue; 
                break;
            }
        }
        return res;
    }
} // namespace leetCode

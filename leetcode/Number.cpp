#include "Number.hpp"

#include <climits>
#include <queue>
#include <math.h>

namespace leetCode
{
    using namespace std;

    int reverse(int num)
    {
        int res = 0;
        while(num != 0)
        {
            if(res < INT_MIN / 10 || res > INT_MAX / 10)
            {
                // 溢出
                return 0;
            }
            res = res * 10 + num % 10;
            num /= 10;
        }
        return res;
    }

    vector<int> numSameConsecDiff(int n, int k)
    {
        deque<int> values;
        // 初始化队列
        for(int i = 1; i < 10; i++)
        {
            values.push_back(i);
        }
        int base = pow(10, n - 1);
        while(values.front() / base == 0)
        {
            int val = values.front();
            values.pop_front();

            // 增加k
            int modVal = val % 10 + k;
            if(k == 0)
            {
                values.push_back(val * 10 + modVal);
            }
            else
            {
                if(modVal < 10)
                {
                    values.push_back(val * 10 + modVal);
                }
            }            
            // 减去k
            if(k != 0)
            {
                modVal = val % 10 - k;
                if(modVal >= 0)
                {
                    values.push_back(val * 10 + modVal);
                }
            }
        }
        return vector<int>(values.begin(), values.end());
    }
    
} // namespace leetCode

#include "Number.hpp"

#include <climits>

namespace leetCode
{
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
} // namespace leetCode

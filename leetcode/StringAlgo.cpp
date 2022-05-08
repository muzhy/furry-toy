#include "StringAlgo.hpp"

#include <map>
#include <assert.h>
#include <algorithm>

namespace leetCode
{
    using namespace std;

    int findMinCharPos(const string& str, int begin, int len)
    {
        assert(begin >= 0);
        assert(begin + len <= str.size());

        int minCharPos = begin;
        for(size_t i = 0; i < len; i++)
        {
            if(str[minCharPos] > str[i + begin])
            {
                minCharPos = i + begin;
            }
        }
        return minCharPos;
    }

    int findMinSubStr(const string& str, int begin, int len)
    {
        assert(begin >= 0);
        assert(begin + len <= str.size());

        int minCharPos = begin;
        for(size_t i = 0; i < len; i++)
        {
            if(str[minCharPos] > str[i + begin])
            {
                minCharPos = i + begin;
            }
            else if(minCharPos != i + begin && str[minCharPos] == str[i + begin])
            {
                size_t oriNextPos = minCharPos + 1;
                size_t newNextPos = i + begin + 1;
                while(oriNextPos != i + begin)
                {
                    if(str[oriNextPos] != str[newNextPos])
                    {
                        minCharPos = str[oriNextPos] < str[newNextPos] ?
                            minCharPos : i + begin;
                        break;
                    }

                    oriNextPos++;
                    newNextPos + 1 < begin + len ?  newNextPos++ : newNextPos = 0;
                }
            }
        }
        return minCharPos;
    }

    string orderlyQueue(string s, int k)
    {
        if(k == 1)
        {
            int minCharPos = findMinSubStr(s, 0, s.size());
            if(minCharPos != 0)
            {
                s = s.substr(minCharPos, s.size() - minCharPos)
                    + s.substr(0, minCharPos);
            }
            
        }
        else
        {
            sort(s.begin(), s.end());
        }
        return s;
    }
}
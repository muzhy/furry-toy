#include <gtest/gtest.h>

#include "Number.hpp"

using namespace std;
using namespace leetCode;

TEST(NumSameConsecDiff, n3k7)
{
    auto res = numSameConsecDiff(3, 7);
    sort(res.begin(), res.end());
    vector<int> expect = {181,292,707,818,929};
    
    EXPECT_EQ(res, expect);
}

TEST(NumSameConsecDiff, n2k1)
{
    auto res = numSameConsecDiff(2, 1);
    sort(res.begin(), res.end());
    vector<int> expect = {10,12,21,23,32,34,43,45,54,56,65,67,76,78,87,89,98};
    
    EXPECT_EQ(res, expect);
}

TEST(NumSameConsecDiff, n2k0)
{
    auto res = numSameConsecDiff(2, 0);
    sort(res.begin(), res.end());
    vector<int> expect = {11,22,33,44,55,66,77,88,99};
    
    EXPECT_EQ(res, expect);
}

TEST(NumSameConsecDiff, n2k2)
{
    auto res = numSameConsecDiff(2, 2);
    sort(res.begin(), res.end());
    vector<int> expect = {13,20,24,31,35,42,46,53,57,64,68,75,79,86,97};
    
    EXPECT_EQ(res, expect);
}
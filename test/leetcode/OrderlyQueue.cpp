#include <gtest/gtest.h>

#include "StringAlgo.hpp"

using namespace leetCode;
using namespace std;

TEST(OrderlyQueueTest, Normal)
{
    EXPECT_EQ("acb", orderlyQueue("cba", 1));
    EXPECT_EQ("aaabc", orderlyQueue("baaca", 3));
}

TEST(OrderlyQueueTest, FullK)
{
    EXPECT_EQ("gvxz", orderlyQueue("gxzv", 4));
}

TEST(OrderlyQueueTest, KUH)
{
    EXPECT_EQ("hku", orderlyQueue("kuh", 1));
}

TEST(OrderlyQueueTest, xmvzi)
{
    EXPECT_EQ("imvxz", orderlyQueue("xmvzi", 2));
}

TEST(OrderlyQueueTest, xxqjzq)
{
    EXPECT_EQ("jqqxxz", orderlyQueue("xxqjzq", 2));
}

TEST(OrderlyQueueTest, nhtq)
{
    EXPECT_EQ("htqn", orderlyQueue("nhtq", 1));
}

TEST(OrderlyQueueTest, TwoMinChar)
{
    EXPECT_EQ(
        "aagtkuqxitavoyjqiupzadbdyymyvuteolyeerecnuptghlzsynozeuuvteryojyokpufanyrqqmtgxhyycltlnusyeyyqygwupc",
        orderlyQueue("xitavoyjqiupzadbdyymyvuteolyeerecnuptghlzsynozeuuvteryojyokpufanyrqqmtgxhyycltlnusyeyyqygwupcaagtkuq", 1));
}

TEST(OrderlyQueueTest, dabeab)
{
    EXPECT_EQ("abdabe", orderlyQueue("dabeab", 1));
}

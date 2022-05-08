#include <iostream>
#include <algorithm>

#include "Number.hpp"

using namespace std;
using namespace leetCode;

int main(int, char**) {

    auto res = numSameConsecDiff(2, 2);
    sort(res.begin(), res.end());
    for(size_t i = 0; i < res.size(); i++)
    {
        cout << res[i] << " ";
    }
    cout << endl;

    return 0;
}

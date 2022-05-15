#include <iostream>
#include <algorithm>

#include "Number.hpp"
#include "slice.hpp"

using namespace std;
using namespace furry_toy;
using namespace leetCode;

int main(int, char**) {
    // ds::slice<int> slice{1, 2, 3, 4};
    // cout << slice.len() << endl;
    // cout << slice.toString() << endl;
    // cout << slice[1] << endl;
    // auto slice2(slice);
    // cout << slice2.toString() << endl;
    // slice[0] = 0;
    // cout << slice.toString() << endl;
    // cout << slice2.toString() << endl;
    // cout << slice.cap() << endl;

    // slice.append({10, 20});
    // cout << slice.toString() << endl;
    // cout << slice2.toString() << endl;

    int intArr[10];
    for(int i = 0; i < 10; i++)
    {
        intArr[i] = i;
    }
    slice<int> s3(intArr, 1, 3);
    cout << s3.toString() << endl;

    slice<int> s4(5);
    cout << s4.toString() << endl;

    return 0;
}

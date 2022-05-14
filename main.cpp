#include <iostream>
#include <algorithm>

#include "Number.hpp"
#include "slice.hpp"

using namespace std;
using namespace furry_toy;
using namespace leetCode;

int main(int, char**) {
    ds::slice<int> slice{1, 2, 3, 4};
    cout << slice.toString() << endl;
    cout << slice[1] << endl;
    auto slice2(slice);
    cout << slice2.toString() << endl;
    slice[0] = 0;
    cout << slice.toString() << endl;
    cout << slice2.toString() << endl;
    cout << slice.cap() << endl;

    slice.append({10, 20});
    cout << slice.toString() << endl;
    cout << slice2.toString() << endl;


    return 0;
}

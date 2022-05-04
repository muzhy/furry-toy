#include <iostream>

#include "TestDemo.hpp"
#include "IntegerReverse.hpp"

using namespace std;

int main(int, char**) {
    cout << leetCode::GetLibName() << endl;

    // cout << leetCode::IntegerReverse(123) << endl;
    // cout << leetCode::IntegerReverse(-123) << endl;
    // cout << leetCode::IntegerReverse(120) << endl;
    // cout << leetCode::IntegerReverse(0) << endl;
    // cout << leetCode::IntegerReverse(1534236469) << endl;
    cout << leetCode::IntegerReverse(-2147483648) << endl;
}

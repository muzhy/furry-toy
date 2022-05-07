#include <iostream>

#include "WinmineSolution.hpp"

using namespace std;
using namespace leetCode;

int main(int, char**) {

    cout << "Hello World" << endl;

    ;

    WinmineSolution::BoardType board = 
    {
        {'E','E','E','E','E'},
        {'E','E','M','E','E'},
        {'E','E','E','E','E'},
        {'E','E','E','E','E'}
    };
    WinmineSolution::BoardPosType clickPos = {3, 0};

    WinmineSolution winmine(board);
    winmine.printBoard();

    winmine.click(clickPos);    
    winmine.printBoard();

    return 0;
}

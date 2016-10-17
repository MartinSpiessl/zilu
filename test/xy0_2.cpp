#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume(x == y && x >= 0);
while(x > 0)
{
recordi(x, y);
x--; y--;
}
recordi(x, y);
iif_assert(y>=0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/xy0_2.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/xy0_2.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/xy0_2.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/xy0_2");
}

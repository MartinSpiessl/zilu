#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume(x == 0 && y == 50);
while(x < 100)
{
recordi(x, y);
x++; if (x > 50) y=y+1;
}
recordi(x, y);
iif_assert(y == 100);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/splitter1.var", loopFunction, "loopFunction", "tmp/splitter1.ds");
context.addLearner("linear");
return context.learn("tmp/splitter1.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/splitter1");
}

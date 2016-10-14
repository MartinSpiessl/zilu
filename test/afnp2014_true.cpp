#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume(x == 1 && y == 0);
while(rand() % 8)
{
recordi(x, y);
 x = x + y; y = y + 1;
}
recordi(x, y);
iif_assert(x >= y);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/afnp2014_true.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/afnp2014_true.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/afnp2014_true.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/afnp2014_true");
}

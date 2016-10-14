#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume( x == y && y >=0);
while(x!=0)
{
recordi(x, y);
x--; y--;
}
recordi(x, y);
iif_assert(y==0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/28_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/28_copy.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/28_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/28_copy");
}

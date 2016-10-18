#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume(x==y);
while(rand() % 8)
{
recordi(x, y);
x=x+y; y=x;
}
recordi(x, y);
iif_assert(y>=1);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/linear1.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/linear1.ds");
context.addLearner("linear");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/linear1.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/linear1");
}

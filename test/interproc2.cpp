#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume(x<y);
while(x<y)
{
recordi(x, y);
 x=x+1;
}
recordi(x, y);
iif_assert(x==y);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/interproc2.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/interproc2.ds");
context.addLearner("linear");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/interproc2.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/interproc2");
}

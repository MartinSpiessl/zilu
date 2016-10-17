#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];

iif_assume(i>=0 && i<=200);
while(i>0)
{
recordi(i);
i--;
}
recordi(i);
iif_assert(i>=0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/bound.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/bound.ds");
context.addLearner("linear");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/bound.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/bound");
}

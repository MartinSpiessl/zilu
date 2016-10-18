#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int j = _reserved_input_[1];

if(i < 0) i = -i; i = i + 1;
iif_assume(i*i < j && i >= 1);
while(i*i < j)
{
recordi(i, j);
 i = i + 1;
}
recordi(i, j);
iif_assert((i - 1) * (i - 1) < j && j <= i * i);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_poly5.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_poly5.ds");
context.addLearner("poly");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_poly5.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_poly5");
}

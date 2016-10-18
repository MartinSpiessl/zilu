#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int j = _reserved_input_[1];
int k = _reserved_input_[2];

iif_assume(i<j && k> 0);
while(i<j)
{
recordi(i, j, k);
 k=k+1;i=i+1;
}
recordi(i, j, k);
iif_assert(k > j - i);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_linear2.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_linear2.ds");
context.addLearner("linear");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_linear2.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_linear2");
}

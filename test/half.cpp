#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int k = _reserved_input_[1];
int n = _reserved_input_[2];

iif_assume(i==0 && k==n && n>=0);
while(i<n)
{
recordi(i, k, n);
k--; i+=2;
}
recordi(i, k, n);
iif_assert(2*k>=n-1);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/half.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/half.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/half.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/half");
}

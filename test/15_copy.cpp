#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int k = _reserved_input_[0];
int j = _reserved_input_[1];
int n = _reserved_input_[2];

iif_assume(n>0 && k>n && j==0);
while(j<n)
{
recordi(k, j, n);
j++; k--;
}
recordi(k, j, n);
iif_assert(k>=0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/15_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/15_copy.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/15_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/15_copy");
}

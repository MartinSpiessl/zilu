#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int k = _reserved_input_[0];
int j = _reserved_input_[1];
int n = _reserved_input_[2];

iif_assume(n>=1 && k>=n && j==0);
while(j<=n-1)
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
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_15.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_15.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_15.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_15");
}

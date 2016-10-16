#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int k = _reserved_input_[1];
int n = _reserved_input_[2];

iif_assume(i==1 && k==1 && n>0);
while(i<=n-1)
{
recordi(i, k, n);
int j=0; while (j<=i-1) {k+=(i-j); j++;} i++;
}
recordi(i, k, n);
iif_assert(k>=n);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_17.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_17.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_17.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_17");
}

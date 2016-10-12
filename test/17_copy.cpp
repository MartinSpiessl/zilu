#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int k = _reserved_input_[1];
int n = _reserved_input_[2];

iif_assume(k==1 && i==1);
while(i<n)
{
recordi(i, k, n);
int j=0; while(j<i) {k+=(i=j); j++;} i++;
}
recordi(i, k, n);
iif_assert(k>=n);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/17_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/17_copy.ds");
context.addLearner("linear");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/17_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/17_copy");
}

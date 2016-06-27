#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int k = _reserved_input_[0];
int n = _reserved_input_[1];

int i=1; int j=0;
iif_assume(k==1);
while(i<n)
{
recordi(k, n);
j=0; while(j<i) {k+=(i=j); j++;} i++;
}
recordi(k, n);
iif_assert(k>=n);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("../tmp//17_copy.var", loopFunction, "loopFunction", "../tmp/17_copy.ds");
context.addLearner("linear");
context.addLearner("poly");
context.addLearner("conjunctive");
return context.learn("../tmp/17_copy.cnt", "../tmp//17_copy");
}

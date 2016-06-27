#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int k = _reserved_input_[0];

int j=0;
int n = rand()%2;
iif_assume(n>0 && k>n);
while(j<n)
{
recordi(k);
j++; k--;
n = rand()%2;
}
recordi(k);
iif_assert(k>=0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("../tmp//15_copy.var", loopFunction, "loopFunction", "../tmp/15_copy.ds");
context.addLearner("linear");
context.addLearner("poly");
context.addLearner("conjunctive");
return context.learn("../tmp/15_copy.cnt", "../tmp//15_copy");
}

#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int j = _reserved_input_[0];
int x = _reserved_input_[1];

int i=0;
iif_assume(i==0 && x==100);
while(i<x)
{
recordi(j, x);
j=j+2;i++;
}
recordi(j, x);
iif_assert(j==2*x);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("../tmp//11_copy.var", loopFunction, "loopFunction", "../tmp/11_copy.ds");
context.addLearner("linear");
context.addLearner("poly");
context.addLearner("conjunctive");
return context.learn("../tmp/11_copy.cnt", "../tmp//11_copy");
}

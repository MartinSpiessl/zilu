#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume(x==y);
while(x!=0)
{
recordi(x, y);
x--; y==
}
recordi(x, y);
iif_assert(y==0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("../tmp//16_copy.var", loopFunction, "loopFunction", "../tmp/16_copy.ds");
context.addLearner("linear");
context.addLearner("poly");
context.addLearner("conjunctive");
return context.learn("../tmp/16_copy.cnt", "../tmp//16_copy");
}

#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int j = _reserved_input_[0];
int k = _reserved_input_[1];

int flag = rand()%2;
iif_assume(j==2 && k==0);
while(rand() % 8)
{
recordi(j, k);
if (flag) j=j+4; else {j=j+2; k=k+1;}
flag = rand()%2;
}
recordi(j, k);
iif_assert(k==0 || j==2*k+2);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("../tmp//13_copy.var", loopFunction, "loopFunction", "../tmp/13_copy.ds");
context.addLearner("linear");
context.addLearner("poly");
context.addLearner("conjunctive");
return context.learn("../tmp/13_copy.cnt", "../tmp//13_copy");
}

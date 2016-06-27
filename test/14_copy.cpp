#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int a = _reserved_input_[0];
int m = _reserved_input_[1];

int j=1;
int flag = rand()%2;
iif_assume(m>0);
while(j<m)
{
recordi(a, m);
if (flag) a++; else a--;
flag = rand()%2;
}
recordi(a, m);
iif_assert(a>=-m && a<=m);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("../tmp//14_copy.var", loopFunction, "loopFunction", "../tmp/14_copy.ds");
context.addLearner("linear");
context.addLearner("poly");
context.addLearner("conjunctive");
return context.learn("../tmp/14_copy.cnt", "../tmp//14_copy");
}

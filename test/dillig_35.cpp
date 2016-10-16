#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int n = _reserved_input_[1];

iif_assume(n>=1 && x==0);
while(x<=n-1)
{
recordi(x, n);
x++;
}
recordi(x, n);
iif_assert(x==n);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_35.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_35.ds");
context.addLearner("linear");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_35.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_35");
}

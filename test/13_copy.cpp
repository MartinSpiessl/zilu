#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int j = _reserved_input_[0];
int k = _reserved_input_[1];
int flag = _reserved_input_[2];

flag = flag % 2;
iif_assume(j==2 && k==0);
while(rand() % 8)
{
recordi(j, k, flag);
if (flag) j=j+4; else {j=j+2; k=k+1;}
}
recordi(j, k, flag);
iif_assert(k==0 || j==2*k+2);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/13_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/13_copy.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/13_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/13_copy");
}

#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int j = _reserved_input_[1];
int x = _reserved_input_[2];

iif_assume(i==0 && j==0 && x>0);
while(i<x)
{
recordi(i, j, x);
j=j+2;i++;
}
recordi(i, j, x);
iif_assert(j==2*x);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/11_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/11_copy.ds");
context.addLearner("linear");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/11_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/11_copy");
}

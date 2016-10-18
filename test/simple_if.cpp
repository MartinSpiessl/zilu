#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int m = _reserved_input_[1];
int n = _reserved_input_[2];

iif_assume(i==1);
while(i<n)
{
recordi(i, m, n);
if(m>0) i=2*i; else i=3*i;
}
recordi(i, m, n);
iif_assert(i>0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/simple_if.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/simple_if.ds");
context.addLearner("linear");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/simple_if.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/simple_if");
}

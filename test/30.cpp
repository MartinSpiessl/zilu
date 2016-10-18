#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int c = _reserved_input_[1];

iif_assume(c==0 && i==0);
while(i<100)
{
recordi(i, c);
c=c+i; i=i+1; if (i<=0) break;
}
recordi(i, c);
iif_assert(c>=0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/30.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/30.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/30.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/30");
}

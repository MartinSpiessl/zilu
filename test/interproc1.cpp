#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];

iif_assume(x<0);
while(x<10 )
{
recordi(x);
 x=x+1;
}
recordi(x);
iif_assert(x==10);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/parallels/Research/GitHub/ZILU/tmp/interproc1.var", loopFunction, "loopFunction", "tmp/interproc1.ds");
context.addLearner("linear");
return context.learn("tmp/interproc1.cnt", "/home/parallels/Research/GitHub/ZILU/tmp/interproc1");
}

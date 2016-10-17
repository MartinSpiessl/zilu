#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];

iif_assume(x==1 || x==2);
while(rand() % 8)
{
recordi(x);
if(x==1) x=2; else if (x==2) x=1;
}
recordi(x);
iif_assert(x<=8);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/pldi08_fig7.c_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/pldi08_fig7.c_copy.ds");
context.addLearner("linear");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/pldi08_fig7.c_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/pldi08_fig7.c_copy");
}

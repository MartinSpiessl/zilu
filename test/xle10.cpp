#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];

iif_assume(x>=0);
while((x>=0) && (x<10) )
{
recordi(x);
 x=x+1;
}
recordi(x);
iif_assert(x>=10);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/xle10.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/xle10.ds");
context.addLearner("linear");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/xle10.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/xle10");
}

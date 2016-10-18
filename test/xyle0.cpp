#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume(x*y>=0);
while(rand() % 8)
{
recordi(x, y);
if(x==0) {if (y>0) x++; else x--;} if(x>0) y++; else x--;
}
recordi(x, y);
iif_assert(x*y>=0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/xyle0.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/xyle0.ds");
context.addLearner("poly");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/xyle0.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/xyle0");
}

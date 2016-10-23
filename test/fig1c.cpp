#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume(x==0 && y==1);
while(rand() % 8)
{
recordi(x, y);
x=x+y; y++;
}
recordi(x, y);
iif_assert(y >= x);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/fig1c.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/fig1c.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/fig1c.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/fig1c");
}

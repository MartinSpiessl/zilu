#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int i = _reserved_input_[2];
int j = _reserved_input_[3];

iif_assume(x==i && y==j);
while(x!=0)
{
recordi(x, y, i, j);
x--; y--;
}
recordi(x, y, i, j);
iif_assert(i!=j || y==0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/16_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/16_copy.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/16_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/16_copy");
}

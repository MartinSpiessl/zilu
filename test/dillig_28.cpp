#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int n = _reserved_input_[2];

iif_assume(x==y && x>=n+1);
while(x>=n+1)
{
recordi(x, y, n);
x--; y--;
}
recordi(x, y, n);
iif_assert(y==n);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_28.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_28.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_28.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_28");
}

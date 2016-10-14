#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int n = _reserved_input_[2];

int m;
m = y;
iif_assume(y>=0 && y<n && x==0);
while(x<n)
{
recordi(x, y, n);
x++; if(x>y) y++;
}
recordi(x, y, n);
iif_assert(y==n);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/parallels/Research/GitHub/ZILU/tmp/19_copy.var", loopFunction, "loopFunction", "/home/parallels/Research/GitHub/ZILU/tmp/19_copy.ds");
context.addLearner("conjunctive");
return context.learn("/home/parallels/Research/GitHub/ZILU/tmp/19_copy.cnt", "/home/parallels/Research/GitHub/ZILU/tmp/19_copy");
}

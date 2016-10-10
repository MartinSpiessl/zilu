#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

int w=1; int z = 0;
iif_assume(x==0 && y==0);
while(rand() % 8)
{
recordi(x, y);
if(w) { x++; w=1-w;} if (!z) {y++; z=1-z;}
}
recordi(x, y);
iif_assert(x==y);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/10_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/10_copy.ds");
context.addLearner("linear");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/10_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/10_copy");
}

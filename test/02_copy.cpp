#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

int z=1; int w = 0;
iif_assume(x==0 && y==0);
while(rand() % 8)
{
recordi(x, y);
 z+=x+y+w; y++; if(z%2==1) x++; w+=2;
}
recordi(x, y);
iif_assert(x==y);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/02_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/02_copy.ds");
context.addLearner("linear");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/02_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/02_copy");
}

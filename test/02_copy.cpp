#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int w = _reserved_input_[2];

int z=1;
iif_assume(x==0 && y==0 && w==0);
while(rand() % 8)
{
recordi(x, y, w);
 z+=x+y+w; y++; if(z%2==1) x++; w+=2;
}
recordi(x, y, w);
iif_assert(x==y);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("../tmp//02_copy.var", loopFunction, "loopFunction", "../tmp/02_copy.ds");
context.addLearner("linear");
context.addLearner("poly");
context.addLearner("conjunctive");
return context.learn("../tmp/02_copy.cnt", "../tmp//02_copy");
}

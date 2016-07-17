#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume(x + y >= 0);
while(y > 0)
{
recordi(x, y);
x++; 
y--;
}
recordi(x, y);
iif_assert(x >= 0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("../tmp/f2.var", loopFunction, "loopFunction", "../tmp/f2.ds");
context.addLearner("linear");
return context.learn("../tmp/f2.cnt", "../tmp/f2");
}

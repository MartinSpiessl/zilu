#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int z = _reserved_input_[2];

iif_assume(x == y && x >= 0 && x+y+z==0);
while(x > 0)
{
recordi(x, y, z);
x--; y--; z++; z++;
}
recordi(x, y, z);
iif_assert(z<=0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/xyz2_2.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/xyz2_2.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/xyz2_2.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/xyz2_2");
}

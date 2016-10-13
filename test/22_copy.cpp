#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int z = _reserved_input_[2];

int randnum = rand()%2;
iif_assume(x==0 && y==0 && z==0);
while(randnum)
{
recordi(x, y, z);
if ((x+y+z)%3==0)x++; y++; z++;
randnum = rand()%2;
}
recordi(x, y, z);
iif_assert(x==y && y==z);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/22_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/22_copy.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/22_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/22_copy");
}

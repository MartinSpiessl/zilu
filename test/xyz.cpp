#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int z = _reserved_input_[2];

iif_assume(x + y + z >= 0);
while(z > 0)
{
recordi(x, y, z);
x++; 
y++;
z-=2;
}
recordi(x, y, z);
iif_assert(x + y >= 0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/xyz.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/xyz.ds");
context.addLearner("linear");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/xyz.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/xyz");
}

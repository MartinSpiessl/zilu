#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int z = _reserved_input_[2];

iif_assume(x==y);
while(rand() % 8)
{
recordi(x, y, z);
x+=10; y+=1;
}
recordi(x, y, z);
iif_assert(x>z || y<z+1);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/xy10.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/xy10.ds");
context.addLearner("linear");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/xy10.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/xy10");
}

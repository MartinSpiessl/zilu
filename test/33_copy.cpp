#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int z = _reserved_input_[2];

iif_assume(x==0 && y==0 && z==0);
while(rand() % 8)
{
recordi(x, y, z);
 int c = 0; while(rand()%4) { if(z==y-c) { x++; y++; c++; }else { x++; y--; c++; } } while(rand()%4) { x--; y--; } z=y;
}
recordi(x, y, z);
iif_assert(x==y);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/33_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/33_copy.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/33_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/33_copy");
}

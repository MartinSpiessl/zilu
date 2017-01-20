#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int z = _reserved_input_[2];

iif_assume(x == y && y == 0 && z==0);
while(rand() % 8)
{
recordi(x, y, z);
x++;y++;z-=2;
}
recordi(x, y, z);
iif_assert(x == y && x >= 0 && x+y+z==0);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/xyz2_1.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/xyz2_1.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/xyz2_1.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/xyz2_1");
}

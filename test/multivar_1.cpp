#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume( y == x);
while(rand() % 8)
{
recordi(x, y);
 x++; y++;
}
recordi(x, y);
iif_assert(x == y);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/multivar_1.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/multivar_1.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/multivar_1.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/multivar_1");
}

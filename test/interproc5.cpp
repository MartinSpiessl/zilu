#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume( x<y);
while( x<y)
{
recordi(x, y);
 x=x+100;
}
recordi(x, y);
iif_assert( x >= y && x <= y + 99);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/interproc5.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/interproc5.ds");
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/interproc5.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/interproc5");
}

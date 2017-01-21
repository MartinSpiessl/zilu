#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];

iif_assume( i < 10 && i > -10);
while( i * i < 100)
{
recordi(i);
 i = i + 1;
}
recordi(i);
iif_assert( i == 10);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/zilu_poly3.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/zilu_poly3.ds");
	context.addLearner("poly");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/zilu_poly3.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/zilu_poly3");
}

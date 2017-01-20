#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int j = _reserved_input_[1];
int r = _reserved_input_[2];

iif_assume(r > i + j);
while(i > 0)
{
recordi(i, j, r);
 i = i - 1; j = j + 1;
}
recordi(i, j, r);
iif_assert(r > i + j);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/zilu_linear3.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/zilu_linear3.ds");
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/zilu_linear3.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/zilu_linear3");
}

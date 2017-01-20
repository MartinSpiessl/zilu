#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int j = _reserved_input_[1];
int k = _reserved_input_[2];

iif_assume(i<j && k>i-j);
while(i<j)
{
recordi(i, j, k);
k=k+1; i=i+1;
}
recordi(i, j, k);
iif_assert(k > 0);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/interproc3.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/interproc3.ds");
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/interproc3.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/interproc3");
}

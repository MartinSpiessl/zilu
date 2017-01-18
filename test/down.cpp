#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int k = _reserved_input_[1];
int n = _reserved_input_[2];

iif_assume(i==0 && k==0);
while(i<n)
{
recordi(i, k, n);
i++; k++;
}
recordi(i, k, n);
iif_assert(k>=n);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/down.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/down.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/down.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/down");
}

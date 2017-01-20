#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int n = _reserved_input_[1];
int sum = _reserved_input_[2];

iif_assume(i==0 && n>=0 && n<=100 && sum==0);
while(i<n)
{
recordi(i, n, sum);
sum = sum + i; i++;
}
recordi(i, n, sum);
iif_assert(sum>=0);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/ex49.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/ex49.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/ex49.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/ex49");
}

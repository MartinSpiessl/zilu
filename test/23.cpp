#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int n = _reserved_input_[0];
int sum = _reserved_input_[1];
int i = _reserved_input_[2];

iif_assume(n>=0 && sum==0 && i==0);
while(i<n)
{
recordi(n, sum, i);
sum=sum+i; i++;
}
recordi(n, sum, i);
iif_assert(sum>=0);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/23.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/23.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/23.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/23");
}

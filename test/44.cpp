#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int j = _reserved_input_[1];
int k = _reserved_input_[2];

iif_assume(i==0 && j==0);
while(i <= k)
{
recordi(i, j, k);
i++; j=j+1;
}
recordi(i, j, k);
iif_assert(j==i);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/44.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/44.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/44.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/44");
}

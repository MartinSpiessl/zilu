#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int j = _reserved_input_[0];
int k = _reserved_input_[1];
int n = _reserved_input_[2];

iif_assume( (j==n) && (k==n) && (n>0));
while(j>0 && n>0)
{
recordi(j, k, n);
j--;k--;
}
recordi(j, k, n);
iif_assert( (k == 0));
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/down_true_2.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/down_true_2.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/down_true_2.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/down_true_2");
}

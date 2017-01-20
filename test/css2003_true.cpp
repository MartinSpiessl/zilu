#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int k = _reserved_input_[1];

iif_assume(0 <= k && k <= 1 && i == 1);
while(rand() % 8)
{
recordi(i, k);
i = i + 1; k = k - 1;
}
recordi(i, k);
iif_assert(1 <= i + k && i + k <= 2 && i >= 1);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/css2003_true.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/css2003_true.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/css2003_true.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/css2003_true");
}

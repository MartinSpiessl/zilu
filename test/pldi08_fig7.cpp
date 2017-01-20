#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];

iif_assume(x==1 || x==2);
while(rand() % 8)
{
recordi(x);
if(x==1) x=2; else if (x==2) x=1;
}
recordi(x);
iif_assert(x<=8);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/pldi08_fig7.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/pldi08_fig7.ds");
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/pldi08_fig7.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/pldi08_fig7");
}

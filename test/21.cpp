#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int n = _reserved_input_[0];
int k = _reserved_input_[1];

int i;
i=0;
int flag = rand()%2;
iif_assume(n>0 && n<10);
while(i<n)
{
recordi(n, k);
i++; if(flag) k+=4000; else k+=2000;
flag = rand()%2;
}
recordi(n, k);
iif_assert(k>n);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/21.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/21.ds");
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/21.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/21");
}

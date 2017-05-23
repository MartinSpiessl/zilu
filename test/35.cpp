#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];

	iif_assume(x>=0);

	while((x>=0) && (x<10) ) {
		recordi(x);
		x=x+1;
		
	}
	recordi(x);

	iif_assert(x>=10);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/35.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/35.ds");
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/35.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/35");
}


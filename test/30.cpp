#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];
	int c = _reserved_input_[1];

	iif_assume( i>=0&&c>=i);

	while(i<40) {
		recordi(i, c);
		c=c+i;
		i++;
		
	}
	recordi(i, c);

	iif_assert(c>=0);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/30.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/30.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/30.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/30");
}


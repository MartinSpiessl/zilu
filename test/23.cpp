#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];
	int j = _reserved_input_[1];

	iif_assume(i==0 && j==0);

	while(i<100) {
		recordi(i, j);
		j+=2;
		i++;
		
	}
	recordi(i, j);

	iif_assert(j==200);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/zilu/tmp/23.var", loopFunction, "loopFunction", "/home/lijiaying/Research/zilu/tmp/23.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/zilu/tmp/23.cnt", "/home/lijiaying/Research/zilu/tmp/23");
}


#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];
	int j = _reserved_input_[1];
	int n = _reserved_input_[2];

	iif_assume(n>=0&&i==0&&j==0);

	while(i<=n) {
		recordi(i, j, n);
		i++;
		j+=i;
		
	}
	recordi(i, j, n);

	iif_assert(i+j>2*n);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/41.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/41.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/41.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/41");
}


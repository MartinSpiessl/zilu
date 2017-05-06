#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int n = _reserved_input_[0];
	int k = _reserved_input_[1];
	int j = _reserved_input_[2];

	iif_assume(n>0&&k>n&&j==0);

	while(j<n) {
		recordi(n, k, j);
		j++;
		k--;
		
	}
	recordi(n, k, j);

	iif_assert(k>=0);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/15.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/15.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/15.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/15");
}


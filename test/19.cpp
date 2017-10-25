#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int j = _reserved_input_[0];
	int k = _reserved_input_[1];
	int n = _reserved_input_[2];

	iif_assume( (j==n) && (k==n) && (n>0));

	while(j>0 && n>0) {
		recordi(j, k, n);
		j--;
		k--;
		
	}
	recordi(j, k, n);

	iif_assert( (k == 0));

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/zilu/tmp/19.var", loopFunction, "loopFunction", "/home/lijiaying/Research/zilu/tmp/19.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/zilu/tmp/19.cnt", "/home/lijiaying/Research/zilu/tmp/19");
}


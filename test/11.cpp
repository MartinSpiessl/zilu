#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];
	int j = _reserved_input_[1];
	int x = _reserved_input_[2];

	iif_assume(j==0&&i==0&&x>=0);

	while(i<x) {
		recordi(i, j, x);
		j=j+2;
		i++;
		
	}
	recordi(i, j, x);

	iif_assert(j==2*x);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/11.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/11.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/11.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/11");
}


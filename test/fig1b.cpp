#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];

	iif_assume( y>0 || x>0);

	while(x+y<=2) {
		recordi(x, y);
;
	}
	recordi(x, y);

	iif_assert(x>0 || y>0);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/fig1b.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/fig1b.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/fig1b.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/fig1b");
}


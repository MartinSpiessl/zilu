#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];

	iif_assume( y == x);

	while(rand()%2) {
		recordi(x, y);
		x++;
		y++;
		
	}
	recordi(x, y);

	iif_assert(x == y);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/zilu/tmp/30.var", loopFunction, "loopFunction", "/home/lijiaying/Research/zilu/tmp/30.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/zilu/tmp/30.cnt", "/home/lijiaying/Research/zilu/tmp/30");
}


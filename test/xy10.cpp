#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];
	int z = _reserved_input_[2];

	iif_assume(x==0&&y==0);

	while(rand()%4) {
		recordi(x, y, z);
		x+=10;
		y++;
		
	}
	recordi(x, y, z);

	iif_assert(x>z||y<z+1);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/xy10.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/xy10.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/xy10.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/xy10");
}


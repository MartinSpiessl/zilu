#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];
	int j = _reserved_input_[1];

	int x, y;
	x = 0; y = 0;
	iif_assume(i==0 && j==0);

	while(rand()%4) {
		recordi(i, j);
		x++;
		y++;
		i+=x;
		j+=y;
		if (flag) j+=1;
		
	}
	recordi(i, j);

	iif_assert(j>=i);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/05.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/05.ds");
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/05.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/05");
}


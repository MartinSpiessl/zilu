#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int n = _reserved_input_[0];
	int x = _reserved_input_[1];
	int y = _reserved_input_[2];

	iif_assume(0<=x&&x<=y&&y<=n);

	while(x<n) {
		recordi(n, x, y);
		x++;
		if(x>y){
		y++;
		}
		
	}
	recordi(n, x, y);

	iif_assert(y==n);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/19.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/19.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/19.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/19");
}


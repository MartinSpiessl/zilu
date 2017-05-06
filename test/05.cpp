#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];
	int i = _reserved_input_[2];
	int j = _reserved_input_[3];

	int flag;
	iif_assume(x==y&&j==i);

	while(rand()%2) {
		recordi(x, y, i, j);
		x++;
		y++;
		i+=x;
		j+=y;
		if(flag)  j+=1;
		
	}
	recordi(x, y, i, j);

	iif_assert(j>=i);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/05.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/05.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/05.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/05");
}


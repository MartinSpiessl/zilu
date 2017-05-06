#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];
	int k = _reserved_input_[2];

	iif_assume((x+y)== k);

	while(rand()%2) {
		recordi(x, y, k);
		if(rand()%2){
		x++;
		y--;
		}
		else{
		y++;
		x--;
		}
		
	}
	recordi(x, y, k);

	iif_assert((x+y)== k);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/20.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/20.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/20.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/20");
}


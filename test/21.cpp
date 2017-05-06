#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int n = _reserved_input_[0];
	int i = _reserved_input_[1];
	int k = _reserved_input_[2];

	iif_assume( k==0&&i==0&&n>0&&n<10);

	while(i<n) {
		recordi(n, i, k);
		i++;
		if(rand()%2==0){
		k+=40;
		}
		else{
		k+=20;
		}
		
	}
	recordi(n, i, k);

	iif_assert(k>n);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/21.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/21.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/21.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/21");
}


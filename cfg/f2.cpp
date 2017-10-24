#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];
	int z = _reserved_input_[2];
	int w = _reserved_input_[3];

	iif_assume(x==0&&y==0&&z==0&&w==0);

	while(rand()%2) {
		recordi(x, y, z, w);
		if(rand()%2){
		x++;
		y=y+2;
		}
		else if(rand()%2){
		if(x>=4){
		x++;
		y=y+3;
		z=z+10;
		w=w+10;
		}
		}
		else if(x>=z&&w>=y+1){
		x=-x;
		y=-y;
		}
		
	}
	recordi(x, y, z, w);

	iif_assert(3*x>y-1);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/f2.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/f2.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/f2.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/f2");
}


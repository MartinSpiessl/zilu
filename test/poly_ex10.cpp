#include "iif.h"
#include <iostream>
using namespace std;
using namespace iif;

int loopFunction(int _reserved_input_[]) {
	int a = _reserved_input_[0];
	int b = _reserved_input_[1];
	int x = _reserved_input_[2];
	int y = _reserved_input_[3];
	int u = _reserved_input_[4];
	int v = _reserved_input_[5];

	iif_assume((x==a) && (y==b) && (u==b) && (v==0));

	while(x!=y) {
		recordi(a, b, x, y, u, v);
		while (x>y) {
		x = x - y;
		v = u + v;
		}
		while (x<y) {
		y = y - x;
		u = u + v;
		}
		
	}
	recordi(a, b, x, y, u, v);

	iif_assert(x*y + y*v - a*b = 0);

	return 0;
}

int main(int argc, char** argv) {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/poly_ex10.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/poly_ex10.ds");
	context.addLearner("poly");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/poly_ex10.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/poly_ex10");
}


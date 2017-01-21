#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];

iif_assume((x>=0) && (x<=50));
while(rand() % 8)
{
recordi(x);
 if (x>50) x++;
		if (x == 0) {
			x ++;
		} else x--;
}
recordi(x);
iif_assert((x>=0) && (x<=50));
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	initseed = atoi(argv[2]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/zilu_poly1.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/zilu_poly1.ds");
	context.addLearner("poly");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/zilu_poly1.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/zilu_poly1");
}

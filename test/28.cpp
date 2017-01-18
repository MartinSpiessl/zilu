#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume( x == y && y >=0);
while(x!=0)
{
recordi(x, y);
x--; y--; if (x<0 || y<0) break;
}
recordi(x, y);
iif_assert(y==0);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/28.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/28.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/28.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/28");
}

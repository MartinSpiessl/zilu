#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int t = _reserved_input_[2];

iif_assume(x!=y && y==t);
while(rand() % 8)
{
recordi(x, y, t);
if(x>0) y=y+x;
}
recordi(x, y, t);
iif_assert(y>=t);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/43.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/43.ds");
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/43.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/43");
}

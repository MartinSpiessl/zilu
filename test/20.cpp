#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int k = _reserved_input_[2];

int j; int i;
j=0;
iif_assume(x+y==k);
while(rand() % 8)
{
recordi(x, y, k);
if(j==i) {x++;y--;} else {y++;x--;} j++; 
}
recordi(x, y, k);
iif_assert(x+y==k);
return 0;
}

int main(int argc, char** argv)
 {
	iifround = atoi(argv[1]);
	iifContext context("/home/lijiaying/Research/GitHub/zilu/tmp/20.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/zilu/tmp/20.ds");
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/tmp/20.cnt", "/home/lijiaying/Research/GitHub/zilu/tmp/20");
}

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
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/20_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/20_copy.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/20_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/20_copy");
}

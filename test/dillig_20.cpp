#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int k = _reserved_input_[2];

int flag = rand()%2;
iif_assume(x+y==k);
while(rand() % 8)
{
recordi(x, y, k);
if (flag) {x++; y--;} else {y++; x--;}
flag = rand()%2;
}
recordi(x, y, k);
iif_assert(x+y==k);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_20.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_20.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_20.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_20");
}

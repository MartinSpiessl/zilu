#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int i = _reserved_input_[2];
int j = _reserved_input_[3];

iif_assume(x==0 && y==0 && i==0 && j==0);
while(rand() % 8)
{
recordi(x, y, i, j);
while(rand()%2){ if (x==y) i++; else j++;} if(i>=j) {x++; y++;} else y++;
}
recordi(x, y, i, j);
iif_assert(i>=j);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/25_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/25_copy.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/25_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/25_copy");
}

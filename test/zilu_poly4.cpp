#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int j = _reserved_input_[1];

iif_assume(i*i < j*j);
while(i*i < j*j)
{
recordi(i, j);
 if (j > 0) { j = j - 1; } else { j = j + 1; }
}
recordi(i, j);
iif_assert(i*i == j*j);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_poly4.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_poly4.ds");
context.addLearner("poly");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_poly4.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_poly4");
}

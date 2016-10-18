#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume( (x*x < y*y) && (y >= 0));
while(x < y)
{
recordi(x, y);
if (x>0) x++; else x--; x=-1 * x;
}
recordi(x, y);
iif_assert((x == y) || (x == y + 1));
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/new_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/new_copy.ds");
context.addLearner("linear");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/new_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/new_copy");
}

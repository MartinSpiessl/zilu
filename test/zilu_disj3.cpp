#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];
int z = _reserved_input_[2];

iif_assume( y>0 || x>0 || z>0);
while(
#loop=if (x>0) x++; else x--; x=-1 * x;
branchcondition=x>0
branch=x++;
branchcondition=y>0
branch=y++;
branchcondition=
branch=z++;)
{
recordi(x, y, z);
}
recordi(x, y, z);
iif_assert(x>0 || y>0 || z>0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_disj3.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_disj3.ds");
context.addLearner("linear");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_disj3.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_disj3");
}

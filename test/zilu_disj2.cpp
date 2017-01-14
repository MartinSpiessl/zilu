#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume( y>0 || x>0);
while(
branchcondition=x>0
branch=x++;
branchcondition=
branch=y++;)
{
recordi(x, y);
}
recordi(x, y);
iif_assert(x>0 || y>0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_disj2.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_disj2.ds");
context.addLearner("linear");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_disj2.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_disj2");
}

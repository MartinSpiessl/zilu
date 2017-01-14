#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume( x<y);
while( x<y)
{
recordi(x, y);

branchcondition=(x<0 && y<0)
branch=x=x+7; y=y-10;
#branchcondition=(x>=0 && y<0)
#branch=x=x+10; y=y-10;
branchcondition=(x<0 && y>=0)
branch=x=x+7; y=y+3;
branchcondition=
branch=x=x+10; y=y+3;
}
recordi(x, y);
iif_assert( x >= y && x <= y + 16);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_disj1.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_disj1.ds");
context.addLearner("linear");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_disj1.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_disj1");
}

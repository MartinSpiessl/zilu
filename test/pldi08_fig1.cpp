#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume(x < 0);
while(1
realloopcondition=x<0)
{
recordi(x, y);

branchcondition=x>=0
branch=break;
branchcondition=
branch=x=x+y; y++;
}
recordi(x, y);
iif_assert(y>=0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/pldi08_fig1.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/pldi08_fig1.ds");
context.addLearner("linear");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/pldi08_fig1.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/pldi08_fig1");
}

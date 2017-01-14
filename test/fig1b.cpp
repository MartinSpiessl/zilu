#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume( y>0 || x>0);
while(1
realloopcondition=x+y<=-2
branchcondition=x+y>-2
branch=break;
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
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/fig1b.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/fig1b.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/fig1b.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/fig1b");
}

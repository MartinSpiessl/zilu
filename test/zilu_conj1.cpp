#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int y = _reserved_input_[1];

iif_assume( x < 100 && y < 100);
while( x < 100 && y < 100 )
{
recordi(x, y);
 x=x+1; y=y+1;
}
recordi(x, y);
iif_assert( x == 100 || y == 100);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/parallels/Research/GitHub/ZILU/tmp/zilu_conj1.var", loopFunction, "loopFunction", "/home/parallels/Research/GitHub/ZILU/tmp/zilu_conj1.ds");
context.addLearner("conjunctive");
return context.learn("/home/parallels/Research/GitHub/ZILU/tmp/zilu_conj1.cnt", "/home/parallels/Research/GitHub/ZILU/tmp/zilu_conj1");
}

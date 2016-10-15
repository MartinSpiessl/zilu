#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int m = _reserved_input_[0];
int n = _reserved_input_[1];

int x = rand()%2;
iif_assume(m==0 && n>0 && x==0);
while(x<n)
{
recordi(m, n);
/*if (flag)*/ m=x; x++;
x = rand()%2;
}
recordi(m, n);
iif_assert(m>=0 && m<n);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/37_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/37_copy.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/37_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/37_copy");
}

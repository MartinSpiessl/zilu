#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int m = _reserved_input_[1];
int n = _reserved_input_[2];

iif_assume(m>=0 && m<=n-1 && x==0);
while(x<=n-1)
{
recordi(x, m, n);
x++; if (x>m) m++;
}
recordi(x, m, n);
iif_assert(m<=n);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_19.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_19.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_19.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_19");
}

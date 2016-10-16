#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int j = _reserved_input_[1];
int n = _reserved_input_[2];

iif_assume(i==0 && j==0 && n>=0);
while(i<n)
{
recordi(i, j, n);
i++; j++;
}
recordi(i, j, n);
iif_assert(j<=n);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_07.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_07.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_07.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_07");
}

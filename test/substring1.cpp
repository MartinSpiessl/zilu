#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int j = _reserved_input_[1];
int k = _reserved_input_[2];

iif_assume(k>=0 && k<=100 && i>=0 && i<=k && j==0);
while(i<k)
{
recordi(i, j, k);
i++; j++;
}
recordi(i, j, k);
iif_assert(j<=100);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/substring1.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/substring1.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/substring1.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/substring1");
}

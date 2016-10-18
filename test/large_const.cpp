#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int k = _reserved_input_[1];
int n = _reserved_input_[2];

int flag = rand()%2;
iif_assume(i==0 && k==0);
while(i<n)
{
recordi(i, k, n);
i++; if(n>99) {k=n+1; break;}if (flag%2==0) k+=4000; else if (flag%2==1) k+=2000; else k+=10000;
flag = rand()%2;
}
recordi(i, k, n);
iif_assert(k>=n);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/large_const.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/large_const.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/large_const.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/large_const");
}

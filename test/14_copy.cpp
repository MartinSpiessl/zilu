#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int a = _reserved_input_[0];
int m = _reserved_input_[1];
int j = _reserved_input_[2];

int flag = rand()%2;
iif_assume(m>0 && a==0 && j==1);
while(j<=m)
{
recordi(a, m, j);
if (flag) a++; else a--; j++;
flag = rand()%2;
}
recordi(a, m, j);
iif_assert(a>=-m && a<=m);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/14_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/14_copy.ds");
context.addLearner("linear");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/14_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/14_copy");
}

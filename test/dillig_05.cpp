#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int j = _reserved_input_[1];
int x = _reserved_input_[2];
int y = _reserved_input_[3];

int flag = rand()%2;
iif_assume(x==y && i==0 && j==0);
while(rand() % 8)
{
recordi(i, j, x, y);
x++; y++; i+=x; j+=y; if (flag) j+=1;
flag = rand()%2;
}
recordi(i, j, x, y);
iif_assert(j>=i);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_05.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_05.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_05.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_05");
}

#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int k = _reserved_input_[0];
int w = _reserved_input_[1];

int flag = rand()%2;
iif_assume(k==1 && w==1);
while(rand() % 8)
{
recordi(k, w);
if (flag > 5) w++; k+=w;
flag = rand()%2;
}
recordi(k, w);
iif_assert(k>0);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_03.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_03.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_03.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/dillig_03");
}

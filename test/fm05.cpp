#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];
int i = _reserved_input_[1];
int j = _reserved_input_[2];

int flag = rand()%2;
iif_assume(x==0 && i==0 && j==0);
while(rand() % 8)
{
recordi(x, i, j);
x++; i+=x; j+=x; if(flag) j+=1;
flag = rand()%2;
}
recordi(x, i, j);
iif_assert(j>=i);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/fm05.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/fm05.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/fm05.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/fm05");
}

#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int j = _reserved_input_[1];

int n; int b;
n = 0; b = rand()%2; 
iif_assume(i==j);
while(n<200)
{
recordi(i, j);
if(b) i++; else j++; b=!b; n++;
}
recordi(i, j);
iif_assert(i==j);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/32_copy.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/32_copy.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/32_copy.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/32_copy");
}

#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int i = _reserved_input_[0];
int j = _reserved_input_[1];

 if (i < 0) i = -i; if (j < 0) j = -j; if (i == 0) i = 1; if (j == 0) j = 1;
iif_assume( i * i < j * j);
while( i < j)
{
recordi(i, j);
 j = j - i; if (j < i) {j = j + i; i = j - i; j = j - i;}
}
recordi(i, j);
iif_assert(j == i);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("../tmp/interproc4.var", loopFunction, "loopFunction", "../tmp/interproc4.ds");
context.addLearner("linear");
return context.learn("../tmp/interproc4.cnt", "../tmp/interproc4");
}

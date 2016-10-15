#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int low = _reserved_input_[0];
int mid = _reserved_input_[1];
int high = _reserved_input_[2];

iif_assume(low == 0 && mid >= 1 && high == 2*mid);
while(mid > 0)
{
recordi(low, mid, high);
low = low + 1; high = high - 1; mid = mid - 1;
}
recordi(low, mid, high);
iif_assert(low == high);
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/cggmp2005_variant_true.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/cggmp2005_variant_true.ds");
context.addLearner("conjunctive");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/cggmp2005_variant_true.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/cggmp2005_variant_true");
}

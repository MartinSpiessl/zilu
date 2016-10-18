#include "iif.h"
#include <iostream>
using namespace iif;

int loopFunction(int _reserved_input_[]) {
int x = _reserved_input_[0];

iif_assume((x>=40) || (x<=0 && x>=-20));
while(rand() % 8)
{
recordi(x);
if(x>=40) x++; else if (x<-20) x--; else if (x==39) x--; else if (x>0) x++;
}
recordi(x);
iif_assert((x>=40) || (x<=0 && x>=-20));
return 0;
}

int main(int argc, char** argv)
 {
iifContext context("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_poly2.var", loopFunction, "loopFunction", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_poly2.ds");
context.addLearner("poly");
return context.learn("/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_poly2.cnt", "/home/lijiaying/Research/GitHub/ZILU/tmp/zilu_poly2");
}

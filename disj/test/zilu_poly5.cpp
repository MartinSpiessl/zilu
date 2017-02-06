#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];
	int j = _reserved_input_[1];

	if(i < 0) i = -i; i = i + 1;
	precondition(i*i < j && i >= 1);

	while(i*i < j) {
		record_variable_int(i, j);
		 
		{
			record_branch(0);
			i = i + 1;
		} 
	}
	record_variable_int(i, j);

	postcondition((i - 1) * (i - 1) < j && j <= i * i);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly5.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly5.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly5.inv");
}


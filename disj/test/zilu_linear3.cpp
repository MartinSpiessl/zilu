#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];
	int j = _reserved_input_[1];
	int r = _reserved_input_[2];

	precondition(r > i + j);

	while(i > 0) {
		record_variable_int(i, j, r);
		 
		{
			record_branch(0);
			i = i - 1; j = j + 1;
		} 
	}
	record_variable_int(i, j, r);

	postcondition(r > i + j);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_linear3.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_linear3.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_linear3.inv");
}


#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];

	precondition( i < 10 && i > -10);

	while( i * i < 100) {
		record_variable_int(i);
		 
		{
			record_branch(0);
			i = i + 1;
		} 
	}
	record_variable_int(i);

	postcondition( i == 10);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly3.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly3.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly3.inv");
}


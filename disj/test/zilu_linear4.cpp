#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int xa = _reserved_input_[0];
	int ya = _reserved_input_[1];

	precondition(xa + ya > 0);

	while(xa > 0) {
		record_variable_int(xa, ya);
		
		{
			record_branch(0);
			xa--; ya++;
		} 
	}
	record_variable_int(xa, ya);

	postcondition(ya >= 0);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_linear4.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_linear4.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_linear4.inv");
}


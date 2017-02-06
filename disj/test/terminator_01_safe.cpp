#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];

	precondition(x>=0);

	while(x<100 && x>=0) {
		record_variable_int(x);
		
		{
			record_branch(0);
			x++;
		} 
	}
	record_variable_int(x);

	postcondition(x>=100);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/terminator_01_safe.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/terminator_01_safe.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/terminator_01_safe.inv");
}


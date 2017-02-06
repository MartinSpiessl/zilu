#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];

	precondition( x<y);

	while( x<y) {
		record_variable_int(x, y);
		{
			record_branch(0);
			 if (x < 0) x = x + 7; else x = x + 10; if (y < 0) y = y - 10; else y = y + 3;
		} 
	}
	record_variable_int(x, y);

	postcondition( x >= y && x <= y + 16);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_linear1.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_linear1.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_linear1.inv");
}


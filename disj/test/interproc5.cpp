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
			x=x+100;
		} 
	}
	record_variable_int(x, y);

	postcondition( x >= y && x <= y + 99);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/interproc5.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/interproc5.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/interproc5.inv");
}


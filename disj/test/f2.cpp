#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];

	precondition(x + y >= 0);

	while(y > 0) {
		record_variable_int(x, y);
		{
			record_branch(0);
			x++; 
		} 
	}

	postcondition(x >= 0);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/disZILU/tmp/f2.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/disZILU/tmp/f2.cnt", "/home/lijiaying/Research/GitHub/disZILU/tmp/f2.inv", "/home/lijiaying/Research/GitHub/disZILU/tmp/f2.pc");
}


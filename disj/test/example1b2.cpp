#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];

	precondition( y>0 || x>0);

	while(x+y<=-2) {
		record_variable_int(x, y);
		{
			record_branch(0);
			if (x>0) x++; else x--; x=-1 * x;
		} 
	}

	postcondition(x>0 || y>0);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/disZILU/tmp/example1b2.var", loopFunction, "loopFunction", NULL);
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/disZILU/tmp/example1b2.cnt", "/home/lijiaying/Research/GitHub/disZILU/tmp/example1b2.inv", "/home/lijiaying/Research/GitHub/disZILU/tmp/example1b2.pc");
}


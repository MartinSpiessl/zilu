#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];

	precondition(x < 0);

	while(x<0) {
		record_variable_int(x, y);
		{
			record_branch(0);
			 x=x+y; y++;
		} 
	}

	postcondition(y>=0);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/disZILU/tmp/pldi08_2.var", loopFunction, "loopFunction", NULL);
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/disZILU/tmp/pldi08_2.cnt", "/home/lijiaying/Research/GitHub/disZILU/tmp/pldi08_2.inv", "/home/lijiaying/Research/GitHub/disZILU/tmp/pldi08_2.pc");
}


#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];
	int z = _reserved_input_[2];

	precondition( y>0 || x>0 || z>0);

	while(rand()%2) {
		record_variable_int(x, y, z);
		if (x>0) {
			record_branch(0);
			x++;
		} else if (y>0) {
			record_branch(1);
			y++;
		} else {
			record_branch(2);
			z++;
		} 
	}

	postcondition(x>0 || y>0 || z>0);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/disZILU/tmp/zilu_disj3.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/disZILU/tmp/zilu_disj3.cnt", "/home/lijiaying/Research/GitHub/disZILU/tmp/zilu_disj3.inv", "/home/lijiaying/Research/GitHub/disZILU/test/zilu_disj3.pc");
}


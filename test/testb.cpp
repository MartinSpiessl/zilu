#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];

	precondition( y>0 || x>0);

	while(rand()%2) {
		record_variable_int(x, y);
		if (x>0) {
			record_branch(0);
			x++;
		} else {
			record_branch(1);
			y++;
		} 
	}
	record_variable_int(x, y);

	postcondition(x>0 || y>0);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/parallels/Research/GitHub/ZILUv2/tmp/testb.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/parallels/Research/GitHub/ZILUv2/tmp/testb.cnt", "/home/parallels/Research/GitHub/ZILUv2/tmp/testb.inv");
}


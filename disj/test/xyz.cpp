#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];
	int z = _reserved_input_[2];

	precondition(x + y + z >= 0);

	while(z > 0) {
		record_variable_int(x, y, z);
		
		{
			record_branch(0);
			x++; y++; z-=2;
		} 
	}
	record_variable_int(x, y, z);

	postcondition(x + y >= 0);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/xyz.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/xyz.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/xyz.inv");
}


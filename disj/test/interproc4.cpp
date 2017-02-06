#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];
	int j = _reserved_input_[1];

	 if (i < 0) i = -i; if (j < 0) j = -j; if (i == 0) i = 1; if (j == 0) j = 1;
	precondition( i * i < j * j);

	while( i < j) {
		record_variable_int(i, j);
		 
		if (j-i<i) {
			record_branch(0);
			i = j - i; j = j - i;
		} else {
			record_branch(1);
			j = j - i;
		} 
	}
	record_variable_int(i, j);

	postcondition(j == i);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/interproc4.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/interproc4.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/interproc4.inv");
}


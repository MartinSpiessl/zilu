#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];
	int j = _reserved_input_[1];

	precondition(i*i < j*j);

	while(i*i < j*j) {
		record_variable_int(i, j);
		 
		if (j > 0) {
			record_branch(0);
			j = j - 1;
		} else {
			record_branch(1);
			j = j + 1;
		} 
	}
	record_variable_int(i, j);

	postcondition(i*i == j*j);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly4.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly4.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly4.inv");
}


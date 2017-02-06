#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];
	int j = _reserved_input_[1];
	int k = _reserved_input_[2];

	precondition(i<j && k>i-j);

	while(i<j) {
		record_variable_int(i, j, k);
		
		{
			record_branch(0);
			k=k+1; i=i+1;
		} 
	}
	record_variable_int(i, j, k);

	postcondition(k > 0);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/interproc3.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/interproc3.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/interproc3.inv");
}


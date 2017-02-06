#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];

	precondition(x==0 && y==50);

	while( x<100) {
		record_variable_int(x, y);
		
		if ((x>=50)) {
			record_branch(0);
			x++; y++;
		} else {
			record_branch(1);
			x++;
		} 
	}
	record_variable_int(x, y);

	postcondition(y==100);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/splitter_fig1.var", loopFunction, "loopFunction", NULL);
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/splitter_fig1.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/splitter_fig1.inv");
}


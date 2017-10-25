#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];

	precondition( x<y);

	while( x<y) {
		record_variable_int(x, y);
		
		if ((x<0 && y<0)) {
			record_branch(0);
			x=x+7; y=y-10;
		} else if ((x<0 && y>=0)) {
			record_branch(1);
			x=x+7; y=y+3;
		} else {
			record_branch(2);
			x=x+10; y=y+3;
		} 
	}

	postcondition( x >= y && x <= y + 16);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/zilu/disj/tmp/zilu_disj1.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/zilu/disj/tmp/zilu_disj1.cnt", "/home/lijiaying/Research/zilu/disj/tmp/zilu_disj1.inv", "/home/lijiaying/Research/zilu/disj/test/zilu_disj1.pc");
}


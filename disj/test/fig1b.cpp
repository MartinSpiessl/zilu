#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];

	precondition( y>0 || x>0);

	while(1) {
		record_variable_int(x, y);
		if (x+y>-2) {
			record_branch(0);
			break;
		} else if (x>0) {
			record_branch(1);
			x++;
		} else {
			record_branch(2);
			y++;
		} 
	}

	postcondition(x>0 || y>0);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/zilu/disj/tmp/fig1b.var", loopFunction, "loopFunction", NULL);
	context.addLearner("conjunctive");
	return context.learn("/home/lijiaying/Research/GitHub/zilu/disj/tmp/fig1b.cnt", "/home/lijiaying/Research/GitHub/zilu/disj/tmp/fig1b.inv", "/home/lijiaying/Research/GitHub/zilu/disj/test/fig1b.pc");
}


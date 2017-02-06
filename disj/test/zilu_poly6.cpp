#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];

	precondition(x*y>=0);

	while(rand()%2) {
		record_variable_int(x, y);
		
		if (x==0 && y>0) {
			record_branch(0);
			x++;
		} else if (x==0) {
			record_branch(1);
			x--;
		} else if (x>0) {
			record_branch(2);
			y++;
		} else {
			record_branch(3);
			x--;
		} 
	}
	record_variable_int(x, y);

	postcondition(x*y>=0);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly6.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly6.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly6.inv");
}


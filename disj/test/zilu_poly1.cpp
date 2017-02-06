#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];

	precondition((x>=0) && (x<=50));

	while(rand()%2) {
		record_variable_int(x);
		
		if (x>50) {
			record_branch(0);
			x++;
		} else if (x==0) {
			record_branch(1);
			x++;
		} else {
			record_branch(2);
			x--;
		} 
	}
	record_variable_int(x);

	postcondition((x>=0) && (x<=50));

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly1.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly1.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly1.inv");
}


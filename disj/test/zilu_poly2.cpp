#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];

	precondition((x>=40) || (x<=0 && x>=-20));

	while(rand()%2) {
		record_variable_int(x);
		
		if (x>=40) {
			record_branch(0);
			x++;
		} else if (x<-20) {
			record_branch(1);
			x--;
		} else if (x==39) {
			record_branch(2);
			x--;
		} else if (x>0) {
			record_branch(3);
			x++;
		} else {
			record_branch(4);
			
		} 
	}
	record_variable_int(x);

	postcondition((x>=40) || (x<=0 && x>=-20));

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly2.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly2.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/zilu_poly2.inv");
}


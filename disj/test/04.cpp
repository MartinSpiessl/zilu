#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];

	precondition(x==-50);

	while(x<0) {
		record_variable_int(x, y);
		{
			record_branch(0);
			x=x+y;y++; 
		} 
	}

	postcondition(y>0);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/zilu/disj/tmp/04.var", loopFunction, "loopFunction", NULL);
	return context.learn("/home/lijiaying/Research/GitHub/zilu/disj/tmp/04.cnt", "/home/lijiaying/Research/GitHub/zilu/disj/tmp/04.inv", "/home/lijiaying/Research/GitHub/zilu/disj/test/04.pc");
}


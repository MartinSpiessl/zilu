#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];
	int z = _reserved_input_[2];

	precondition(x==y);

	while(rand()%2) {
		record_variable_int(x, y, z);
		{
			record_branch(0);
			x+=10; y+=1;
		} 
	}
	record_variable_int(x, y, z);

	postcondition(x>z || y<z+1);

	return 0;
}

int main(int argc, char** argv) {
	Context context("/home/lijiaying/Research/GitHub/ZILUv2/tmp/xy10.var", loopFunction, "loopFunction", NULL);
	context.addLearner("linear");
	return context.learn("/home/lijiaying/Research/GitHub/ZILUv2/tmp/xy10.cnt", "/home/lijiaying/Research/GitHub/ZILUv2/tmp/xy10.inv");
}


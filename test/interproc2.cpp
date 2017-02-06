#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int x = _reserved_input_[0];
	int y = _reserved_input_[1];

	precondition(x<y);

	while(x<y) {
		record_variable_int(x, y);
		x=x+1;
		
	}
	record_variable_int(x, y);

	postcondition(x==y);

	return 0;
}

int main(int argc, char** argv) {
	 iifround = atoi(argv[1]);
	 iifseed = atoi(argv[2]);
	Context context("


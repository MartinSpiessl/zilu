#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];
	int c = _reserved_input_[1];

	precondition(c==0 && i==0);

	while(i<100) {
		record_variable_int(i, c);
		c=c+i;
		i=i+1;
		if (i<=0) break;
		
	}
	record_variable_int(i, c);

	postcondition(c>=0);

	return 0;
}

int main(int argc, char** argv) {
	 iifround = atoi(argv[1]);
	 iifseed = atoi(argv[2]);
	Context context("


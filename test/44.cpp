#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];
	int j = _reserved_input_[1];
	int k = _reserved_input_[2];

	precondition(i==0 && j==0);

	while(i <= k) {
		record_variable_int(i, j, k);
		i++;
		j=j+1;
		
	}
	record_variable_int(i, j, k);

	postcondition(j==i);

	return 0;
}

int main(int argc, char** argv) {
	 iifround = atoi(argv[1]);
	 iifseed = atoi(argv[2]);
	Context context("


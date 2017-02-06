#include "record.h"
#include <iostream>
using namespace std;
using namespace record;

int loopFunction(int _reserved_input_[]) {
	int i = _reserved_input_[0];
	int n = _reserved_input_[1];
	int sum = _reserved_input_[2];

	precondition(i==0 && n>=0 && n<=100 && sum==0);

	while(i<n) {
		record_variable_int(i, n, sum);
		sum = sum + i;
		i++;
		
	}
	record_variable_int(i, n, sum);

	postcondition(sum>=0);

	return 0;
}

int main(int argc, char** argv) {
	 iifround = atoi(argv[1]);
	 iifseed = atoi(argv[2]);
	Context context("


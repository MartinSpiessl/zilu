#include "config.h"
#include "record.h"
#include "color.h"
#include "instrumentation.h"
#include <iostream>
#include <stdlib.h>

//extern int assume_times, assert_times;
int(*target_program)(int*) = NULL;

int minv = -1 * base_step, maxv = base_step;
std::string* variables;
VariablePowerArray* vparray;
int vnum;
#ifdef __PRT_STATISTICS
int random_samples = 0, selective_samples = 0;
#endif

/*
bool check_target_program(int (*func)(int*))
{
    Solution sol;
    Polynomial::solver(NULL, sol);
	int a[Nv];
	for (int i = 0; i < Nv; i++)
	    a[i] = sol[i];
	assume_times = 0;
	assert_times = 0;
	func(a);
	if (assume_times != 1)
		return false;
	if (assert_times != 1)
		return false;
	return true;
}
*/

std::string labelString(int label) {
	std::ostringstream stm;
	switch (label) {
		case NEGATIVE:
			stm << RED << "-" << NORMAL;
			break;
		case QUESTION:
			stm << BLUE << "?" << NORMAL;
			break;
		case CNT_EMPL:
			stm << YELLOW << "x" << NORMAL;
			break;
		default:
			stm << GREEN << "+" << NORMAL; //<< label-POSITIVE << NORMAL;
	}
	return stm.str(); 
}

bool register_program(int (*func)(int*), const char* func_name) 
{
	/*if (check_target_program(func) == false) {
		if (func_name == NULL) {
			std::cout << "The target is not a valid program to test.\n";
		} else {
			std::cout << func_name << " is not a valid program to test.\n";
		}
		return false;
	}
	*/
	target_program = func;
	return true;
}

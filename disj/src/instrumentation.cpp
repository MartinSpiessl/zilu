#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include "instrumentation.h"
#include <assert.h>
//#include "color.h"

bool _passP = false;
bool _passQ = false;
//unsigned int _branch = 0;
//int assume_times = 0;
//int assert_times = 0;
char lt[4][10] =  { "Negative", "Question", "Bugtrace", "Positive"};
char(*LabelTable)[10] = &lt[1];

double program_states[MstatesIn1trace * 2][Nv];
int program_branches[MstatesIn1trace];
int state_index;



void record_branch(int b) {
	//_branch = b;
	program_branches[state_index] = b;
	if (b >= 32) {
		std::cout << RED << "more branches" << b <<" than supported(32).\n" << NORMAL;
		exit(-1);
	}
	state_index++;
	if (state_index >= MstatesIn1trace) {
		std::cout << RED << "\nToo many states (>" << MstatesIn1trace << 
			") in one execution. Stop here.\n" << NORMAL;
		exit(-1);
	}
}

int addStateInt(int first ...)
{
	if (state_index >= 0.9 * MstatesIn1trace)
		if (rand() % (100 * state_index / MstatesIn1trace) > 1)
			return 0;
	if (state_index >= 0.999 * MstatesIn1trace)
		return 0;
	va_list ap;
	va_start(ap, first);
	program_states[state_index][0] = first;
	for (int i = 1; i < Nv; i++) {
		program_states[state_index][i] = va_arg(ap, int);
	}
	va_end(ap);

#if 0
#ifdef __PRT_TRACE
	std::cout << BLUE << "(" << program_states[state_index][0];
	for (int i = 1; i < Nv; i++) {
		std::cout << "," << program_states[state_index][i];
	}
	std::cout << ")" << NORMAL;
#endif

	state_index++;
	if (state_index >= MstatesIn1trace) {
		std::cout << RED << "\nToo many states (>" << MstatesIn1trace << 
			") in one execution. Stop here.\n" << NORMAL;
		exit(-1);
	}
#endif
	return 0;
}

int addStateDouble(double first, ...)
{
	if (state_index >= 0.9 * MstatesIn1trace)
		if (rand() % (100 * state_index / MstatesIn1trace) > 1)
			return 0;
	if (state_index >= 0.999 * MstatesIn1trace)
		return 0;
	va_list ap;
	va_start(ap, first);
	program_states[state_index][0] = first;
	for (int i = 1; i < Nv; i++) {
		program_states[state_index][i] = va_arg(ap, double);
	}
	va_end(ap);

#if 0
#ifdef __PRT_TRACE
	std::cout << BLUE << "(" << program_states[state_index][0];
	for (int i = 1; i < Nv; i++) {
		std::cout << "," << program_states[state_index][i];
	}
	std::cout << ")" << NORMAL;
#endif

	state_index++;
	if (state_index >= MstatesIn1trace) {
		std::cout << RED << "\nToo many states (>" << MstatesIn1trace << 
			") in one execution. Stop here.\n" << NORMAL;
		exit(-1);
	}
#endif
	return 0;
}


int beforeLoop()
{
	//std::cout << "---> before_loop";
	state_index = 0;
	_passP = false;
	_passQ = false;
	/*if (Nb == 1) {
		program_branches[0] = 0; // compatible to old ZILU
	} else {
		program_branches[0] = Nb+2; // any integer not used...
	}
	*/
	//_branch = 0;
	//assume_times = 0;
	//assert_times = 0;
	//std::cout << "[done]";
	return 0;
}


int afterLoop(States* gsets)
{
	int label = 0;
	//assert(assume_times == 1);
	//assert(assert_times == 1);
	if (_passP && _passQ) {
		//label = POSITIVE;
		label = POSITIVE;
	} else if (!_passP && !_passQ) {
		label = NEGATIVE; 
	} else if (!_passP && _passQ) {
#ifdef __QUESTION_AS_POSITIVE
		//std::cout << "?->+ ";
		label = POSITIVE; 
#elif __QUESTION_AS_NEGATIVE
		//std::cout << "?->- ";
		label = NEGATIVE;
#else
		//std::cout << "?->? ";
		label = QUESTION; 
#endif
		//label = POSITIVE; 
	} else if (_passP && !_passQ) {
		label = CNT_EMPL;
		std::cout << RED << "\ncounter-example trace:  ";
		for (int i = 0; i < state_index; i++) {
			std::cout << "(" << program_states[i][0];
			for (int j = 1; j < Nv; j++)
				std::cout << "," << program_states[i][j];
			std::cout << ")->";
		}
		std::cout << "END[x]" << NORMAL << std::endl;
	}

#ifdef __PRT_TRACE
	Color c;
	switch (label) {
		case POSITIVE:
			c = GREEN;
			break;
		case NEGATIVE:
			c = RED;
			break;
		case QUESTION:
			c = BLUE;
			break;
	}
	std::cout << YELLOW << "@";
	record_branch(0);
	for (int i = 0; i < state_index; i++) {
		if (i == 0) 
			std::cout << c << "oo" << "(" << program_states[i][0];
		else
			std::cout << c << "b" << program_branches[i] << "(" << program_states[i][0];
		for (int j = 1; j < Nv; j++)
			std::cout << "," << program_states[i][j];
		std::cout << ") -> ";
	}
	std::cout << /*"END" <<*/ "[" << labelString(label) << c << "]" << NORMAL; // << std::endl;
#endif

	//std::cout << "----> add-states to gsets[" << label << "]" << std::endl;
	//if (label == POSITIVE || label == NEGATIVE || label == QUESTION)
		gsets[label].addStates(program_states, program_branches, state_index);
		//gsets[label].addStates(program_states, program_branches + 1, state_index);
	//std::cout << "<---- add-states to gsets[" << label << "]" << std::endl;
	//std::cout << "<---- add-states to gsets[" << label << "]" << std::endl;
	return label;
}

void printRunResult(int rr) {
	std::cout << labelString(rr);
	return;
}

int mDouble(double* p)
{
	int a[Nv];
	for (int i = 0; i < Nv; i++)
		a[i] = static_cast<int>(p[i]);
	return mInt(a);
}

int mInt(int* p) { return target_program(p); }

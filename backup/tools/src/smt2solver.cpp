/***************************************************************
 *  @file smt2solver.cpp
 *  @brief           
 *             
 *  @author Li Jiaying
 *  @bug no known bugs
 ***************************************************************/
#include <z3++.h>
#include <iostream>

using namespace std;
using namespace z3;

int main(int argc, char** argv) {
	assert(argc >= 2 && __FILE__"need more parameters.\n");
	char* smt2file = argv[1];
	context c;
	solver s(c);
	Z3_ast f = Z3_parse_smtlib2_file(c, smt2file, 0, 0, 0, 0, 0, 0);
	expr r = to_expr(c, f);
	s.add(r);
	if (s.check() == sat) {
		cout << "sat" << endl;
		cout << s.get_model() << endl;
		return 0;
	} else {
		cout << "unsat" << endl;
		return 1;
	}

	return 0;
}

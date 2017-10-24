#include <iostream>
#include <sstream>
#include <cassert>
#include "node.hpp"
using namespace std;

class LoopHandler {
	public:
		Loop* loop;
		LoopHandler(Loop* loop) {
			this->loop = loop;
		}
		std::string toCFile() {
			// plain c file, with only one loop function...
			std::ostringstream stm;
			stm << toInclude() << "\n"
				<< "#define precondition assume\n"
				<< "#define postcondition assert\n"
				<< toLoopFunctionHeader() << "\n"
				<< toLoopFunctionBody() << "\n"
				<< toLoopFunctionFooter() << "\n";
			return stm.str();
		}

		std::string toReverseBranchFile4Klee() {
			// used for selective sampling, from record_variable back to the program entrance...
			//assert(b>=0 && b<loop->loopbody->n);
			//cout << "--------> toReverseBranchFile4Klee\n";
			std::ostringstream stm;
			stm << "#include \"../include/reverse_execution/candidate.h\"\n"
				<< toKleeInclude() << "\n"
				<< toKleeReverseExecutionMain() << "\n";
			//cout << "<-------- toReverseBranchFile4Klee\n";
			return stm.str();
		}

		std::string toCFileWithLoopRecord(const char* cefile = NULL, const char* varfile = NULL, const char* invfile = NULL, const char* dsfile = NULL) {
		//std::string toCFileWithLoopRecord() { //const char* cefile = "default.ce", const char* varfile = "default.var", const char* invfile = "default.inv") {
			// used for the previous setting, which we do not care about the intra-structure in the loop
			// only record the pass/fail of precondition and postcondition
			std::ostringstream stm;
			stm << toRecordInclude() << "\n"
				//<< "#define b_true do {record_path(depth,1);} while(0)\n"
				//<< "#define b_false do {record_path(depth,0);} while(0)\n\n"
				<< toLoopFunctionHeader() << "\n"
				<< toLoopFunctionBodyWithLoopRecord() << "\n"
				<< toLoopFunctionFooter() << "\n"
				<< toMainFunctionWithSeed(cefile, varfile, invfile, dsfile) << "\n"; //cefile, varfile, invfile) << "\n";
			return stm.str();
		}

		std::string toCFileWithLoopBranchesRecord(const char* cefile = "default.ce", const char* varfile = "default.var", const char* invfile = "default.inv") {
			// used for our current setting
			// it can record branch info along with the pass/fail of precondition and postcondition
			std::ostringstream stm;
			stm	<< "#include \"assume.h\"\n"
				<< toRecordInclude()
				//<< "#define assume(expr) if(expr) {record_path(depth, 1);} else {record_path(depth, 0);} if(expr)\n" 
				<< "int depth;\n\n"
				//<< "#define Depth(n) do {depth=n;} while(0)\n\n"
				<< toLoopFunctionHeader() << "\n"
				<< toLoopFunctionBodyWithLoopBranchesRecord() << "\n"
				<< toLoopFunctionFooter() << "\n"
				//<< toMainFunction(cefile, varfile, invfile) << "\n";
				<< toMainFunction() << "\n";
			return stm.str();
		}

		std::string toCFile4Verification(int choice) {
			// used for verifying the invariant candidate
			std::ostringstream stm;
			stm << toKleeInclude() << "\n"
				<< "#define Depth(n) do {} while(0)\n\n"
				<< toKleeMain4Verification(choice);
			return stm.str();
		}

		std::string toCFile4Init(int positive) {
			// used for verifying the invariant candidate
			std::ostringstream stm;
			//cout << "in cfile 4 init.\n";
			stm << toKleeInclude() << "\n"
				<< toKleeMain4Init(positive);
			return stm.str();
		}

			private:
		std::string toLoopBodyWithReturn(int tabs) {
			if (loop->loopbody == NULL)
				return ";";
			ostringstream stm;
			std::size_t last = -1;
			for (int i = 0; i < loop->symbs->n; i++) {
				for (int j=0; j<tabs; j++) {
					stm << "\t";
				}
				stm << "\tint " << loop->symbs->u[i] << " = rand() % 2;\n";
			}
			while (true) {
				for (int i=0; i<tabs; i++)
					stm << "\t";
				std::size_t first = last + 1;
				while (loop->loopbody->find_first_of(' ', first) == first)
					first++;
				last = loop->loopbody->find_first_of(';', first);
				std::size_t last1 = loop->loopbody->find_first_of('{', first);
				if (last1 != std::string::npos && last1 < last) last = last1;
				std::size_t last2 = loop->loopbody->find_first_of('}', first);
				if (last2 != std::string::npos && last2 < last) last = last2;
				if (last != std::string::npos) {
					stm << loop->loopbody->substr(first, last - first + 1) << "\n";
				} else {
					stm << loop->loopbody->substr(first);
					break;
				}
			}
			return stm.str();
		}

		std::string toInclude() {
			std::ostringstream stm;
			stm << "#include <iostream>\n" 
				<< "using namespace std;\n\n";
			return stm.str();
		}

		std::string toRecordInclude() {
			std::ostringstream stm;
			stm << "#include \"iif.h\"\n"
				<< "#include <iostream>\n" 
				<< "using namespace std;\n"
				<< "using namespace iif;\n";
			return stm.str();
		}

		std::string toKleeInclude() {
			std::ostringstream stm;
			stm << "#include <klee/klee.h>\n\n";
			return stm.str();
		}

		std::string toKleeMain4Verification(int choice) {
			std::ostringstream stm;
			stm << "int main() {\n"; 
			for (int i = 0; i < loop->names->n; i++) 
				stm << "\tint " << loop->names->u[i] << ";\n";
			for (int i = 0; i < loop->symbs->n; i++) 
				stm << "\tint " << loop->symbs->u[i] << ";\n";
			for (int i = 0; i < loop->names->n; i++) 
				stm << "\tklee_make_symbolic(&" << loop->names->u[i] <<", sizeof(" << loop->names->u[i] << "), \"" << loop->names->u[i] << "\");\n";
			for (int i = 0; i < loop->symbs->n; i++) 
				stm << "\tklee_make_symbolic(&" << loop->symbs->u[i] <<", sizeof(" << loop->symbs->u[i] << "), \"" << loop->symbs->u[i] << "\");\n";
			// before loop statements;
			if (loop->beforeloop && loop->beforeloop->compare("") != 0)
				stm << "\t" << *loop->beforeloop << "\n";

			switch (choice) {
				case 1:
					if (loop->beforeloopinit && loop->beforeloopinit->compare("") != 0)
						stm << "\t" << *loop->beforeloopinit << "\n";
					stm << "\tklee_assume(" << *loop->precondition <<");\n"; 
					stm << "\tklee_fail(" << *loop->loopinvariant <<");\n"; 
					break;
				case 2:
					if (loop->beforeloopinit && loop->beforeloopinit->compare("") != 0)
						stm << "\t" << *loop->beforeloopinit << "\n";
					if (loop->loopcondition && loop->loopcondition->compare("") != 0)
						stm << "\tklee_assume(" << *loop->loopcondition <<");\n"; 
					stm << "\tklee_assume(" << *loop->loopinvariant <<");\n"; 
					stm << "\tdo {\n";
					if (loop->loopbody) {
						stm << "{\n" << toLoopBodyWithReturn(2) << "\n\t} \n"; 
					}
					stm << "\t} while(0);\n";
					stm << "\tklee_fail(" << *loop->loopinvariant <<");\n"; 
					break;
				case 3:
					if (loop->loopcondition && loop->loopcondition->compare("") != 0)
						stm << "\tklee_assume(!(" << *loop->loopcondition <<"));\n"; 
					stm << "\tklee_assume(" << *loop->loopinvariant <<");\n"; 
					stm << "\tklee_fail(" << *loop->postcondition <<");\n"; 
					break;
			}
			stm << "\treturn 0;\n}" << endl;
			return stm.str();
		}

		std::string toKleeMain4Init(int positive) {
			std::ostringstream stm;
			stm << "int main() {\n"; 
			for (int i = 0; i < loop->names->n; i++) 
				stm << "\tint " << loop->names->u[i] << ";\n";
			for (int i = 0; i < loop->symbs->n; i++) 
				stm << "\tint " << loop->symbs->u[i] << ";\n";
			for (int i = 0; i < loop->names->n; i++) 
				stm << "\tklee_make_symbolic(&" << loop->names->u[i] <<", sizeof(" << loop->names->u[i] << "), \"" << loop->names->u[i] << "\");\n";
			for (int i = 0; i < loop->symbs->n; i++) 
				stm << "\tklee_make_symbolic(&" << loop->symbs->u[i] <<", sizeof(" << loop->symbs->u[i] << "), \"" << loop->symbs->u[i] << "\");\n";
			// before loop statements;
			if (loop->beforeloop && loop->beforeloop->compare("") != 0)
				stm << "\t" << *loop->beforeloop << "\n";

			if (loop->beforeloopinit && loop->beforeloopinit->compare("") != 0)
				stm << "\t" << *loop->beforeloopinit << "\n";
			if (loop->precondition) {
				if (positive >= 0) {
					stm << "\tklee_pass(" << *loop->precondition <<");\n"; 
				} else {
					stm << "\tklee_fail(" << *loop->precondition <<");\n"; 
				}
			} else {
				stm << "\tklee_pass(" << 1 <<");\n"; 
			}

			stm << "\treturn 0;\n}" << endl;
			return stm.str();
		}

		std::string toKleeReverseExecutionMain() {
			//cout << " -----> toKleeReverseExecutionMain(" << b << ")\n";
			std::ostringstream stm;
			stm << "int main() {\n"; 
			for (int i = 0; i < loop->names->n; i++) 
				stm << "\tint " + loop->names->u[i] + ";\n";
			for (int i = 0; i < loop->symbs->n; i++) 
				stm << "\tint " << loop->symbs->u[i] << ";\n";
			for (int i = 0; i < loop->names->n; i++) 
				stm << "\tklee_make_symbolic(&" << loop->names->u[i] <<", sizeof(" << loop->names->u[i] << "), \"" << loop->names->u[i] << "\");\n";
			for (int i = 0; i < loop->symbs->n; i++) 
				stm << "\tklee_make_symbolic(&" << loop->symbs->u[i] <<", sizeof(" << loop->symbs->u[i] << "), \"" << loop->symbs->u[i] << "\");\n";
			// before loop statements;
			if (loop->beforeloop && loop->beforeloop->compare("") != 0)
				stm << "\t" << *loop->beforeloop << "\n";

			if (loop->beforeloopinit && loop->beforeloopinit->compare("") != 0)
				stm << "\t" << *loop->beforeloopinit << "\n";
			if (loop->loopcondition && loop->loopcondition->compare("") != 0)
				stm << "\tklee_assume(" << *loop->loopcondition <<");\n"; 
			stm << "\tdo {\n";
			stm << toLoopBodyWithReturn(2) << "\n";
			stm << "\t} while(0);\n";
			//stm << "\tklee_pass(Candidate" << b << ");\n"; 
			stm << "\tklee_pass(Candidate);\n"; 
			stm << "\treturn 0;\n}";
			return stm.str();
		}

		std::string toLoopFunctionHeader() {
			std::ostringstream stm;
			stm <<"int loopFunction(int _reserved_input_[]) {\n";
			for (int i = 0; i < loop->names->n; i++) {
				stm << "\tint " << loop->names->u[i] << " = _reserved_input_[" << i << "];\n";
			}
			return stm.str();
		}

		std::string toLoopFunctionBody() {
			std::ostringstream stm;
			stm << toBeforeLoopBody();
			stm << toLoopBodyWithReturn(2) << endl;
			stm << "\t}\n";
			stm << toAfterLoopBody();
			return stm.str();
		}

		std::string toRecordVariables() {
			std::ostringstream stm;
			stm << "recordi(" << loop->names->u[0];
			for (int i = 1; i < loop->names->n; i++)
				stm << ", " << loop->names->u[i];
			stm << ");";
			return stm.str();
		}

		std::string toLoopFunctionBodyWithLoopRecord() {
			std::ostringstream stm;
			stm << toBeforeLoopBody();
			stm << "\t\t" << toRecordVariables() << endl;
			stm << toLoopBodyWithReturn(2) << endl;
			stm << "\t}\n";
			stm << "\t" << toRecordVariables() << endl;
			stm << toAfterLoopBody();
			return stm.str();
		}

		std::string toLoopFunctionBodyWithLoopBranchesRecord() {
			std::ostringstream stm;
			stm << toBeforeLoopBody();
			stm << "\t\t" << toRecordVariables() << endl;
			stm << toLoopBodyWithReturn(2) << endl;
			stm << "\t}\n";
			stm << "\t" << toRecordVariables() << endl;
			stm << toAfterLoopBody();
			return stm.str();
		}

		std::string toBeforeLoopBody() {
			std::ostringstream stm;
			if (loop->beforeloop && *loop->beforeloop != "") 
				stm << "\t" << *loop->beforeloop << endl;
			if (loop->beforeloopinit && *loop->beforeloopinit != "") 
				stm << "\t" << *loop->beforeloopinit << endl;
			if (loop->precondition) 
				//stm << "\t/***precondition***/\n\tprecondition(" << *loop->precondition << ");" << endl << endl;
				stm << "\tiif_assume(" << *loop->precondition << ");" << endl << endl;
			if (loop->loopcondition && *loop->loopcondition != "") 
				stm << "\twhile(" << *loop->loopcondition << ") {" << endl;
			else 
				//stm << "\twhile(non_det()) {" << endl;
				stm << "\twhile(rand()%2) {" << endl;
			return stm.str();
		}

		std::string toAfterLoopBody() {
			std::ostringstream stm;
			if (loop->postcondition) 
				//stm << "\n\t/***postcondition***/\n\tpostcondition(" << *loop->postcondition << ");" << endl << endl;
				stm << "\n\tiif_assert(" << *loop->postcondition << ");" << endl << endl;
			stm << "\treturn 0;";
			return stm.str();
		}

		std::string toLoopFunctionFooter() {
			return string("}\n");
		}

		std::string toMainFunction() { //const char* cefile, const char* varfile, const char* invfile) {
			std::ostringstream stm;
			stm << "int main(int argc, char** argv) {\n";
			//stm << "\tContext context(\"" << varfile <<"\", loopFunction, \"loopFunction\", NULL);\n";
			stm << "\tContext context(argv[1], loopFunction, \"loopFunction\", NULL);\n";

			{
				//std::cout << "Learners [["; // "size=" << learners.size() << std::endl;
				int size = loop->learners->n;
				if (size == 0 || loop->learners->u[0].compare("") == 0 || loop->learners->u[0].find("def") != string::npos) {
					//std::cout << "--> linear --> polynomial --> conjunctive";
					stm << "\tcontext.addLearner(\"linear\");\n";
					stm << "\tcontext.addLearner(\"poly\");\n";
					stm << "\tcontext.addLearner(\"conjunctive\");\n";
				} else {
					for (int i = 0; i < size; i++) {
						if (loop->learners->u[i].find("lin") != string::npos) {
							//std::cout << "--> linear";
							stm << "\tcontext.addLearner(\"linear\");\n";
						} else if (loop->learners->u[i].find("poly") != string::npos) {
							//std::cout << "--> polynomial";
							stm << "\tcontext.addLearner(\"poly\");\n";
						} else if (loop->learners->u[i].find("conj") != string::npos) {
							//std::cout << "--> conjunctive";
							stm << "\tcontext.addLearner(\"conjunctive\");\n";
						}
					}
				}
				//std::cout << "]]";
			}

			stm << "\treturn context.learn(argv[2], argv[3]);" << endl;
			stm << "}";
			return stm.str();
		}

		std::string toMainFunctionWithSeed(const char* cefile, const char* varfile, const char* invfile, const char* dsfile) {
			std::cout << "parsing>>>>" << cefile << " - " << varfile << " - " << invfile << " - " << dsfile << std::endl; 
			std::ostringstream stm;
			stm << "int main(int argc, char** argv) {\n";
			stm << "\tiifround = atoi(argv[1]);\n";
			stm << "\tinitseed = atoi(argv[2]);\n";
			if (dsfile != NULL)
				stm << "\tiifContext context(\"" << varfile <<"\", loopFunction, \"loopFunction\", \"" << dsfile << "\");\n";
			else 
				stm << "\tiifContext context(\"" << varfile <<"\", loopFunction, \"loopFunction\", NULL);\n";
			//stm << "\tContext context(argv[1], loopFunction, \"loopFunction\", NULL);\n";

			{
				//std::cout << "Learners [["; // "size=" << learners.size() << std::endl;
				int size = loop->learners->n;
				if (size == 0 || loop->learners->u[0].compare("") == 0 || loop->learners->u[0].find("def") != string::npos) {
					//std::cout << "--> linear --> polynomial --> conjunctive";
					stm << "\tcontext.addLearner(\"linear\");\n";
					stm << "\tcontext.addLearner(\"poly\");\n";
					stm << "\tcontext.addLearner(\"conjunctive\");\n";
				} else {
					for (int i = 0; i < size; i++) {
						if (loop->learners->u[i].find("lin") != string::npos) {
							//std::cout << "--> linear";
							stm << "\tcontext.addLearner(\"linear\");\n";
						} else if (loop->learners->u[i].find("poly") != string::npos) {
							//std::cout << "--> polynomial";
							stm << "\tcontext.addLearner(\"poly\");\n";
						} else if (loop->learners->u[i].find("conj") != string::npos) {
							//std::cout << "--> conjunctive";
							stm << "\tcontext.addLearner(\"conjunctive\");\n";
						}
					}
				}
				//std::cout << "]]";
			}

			if (cefile)
				stm << "\treturn context.learn(\"" << cefile << "\", \"" << invfile << "\");" << endl;
			else
				stm << "\treturn context.learn(NULL, \"" << invfile << "\");" << endl;
			stm << "}";
			return stm.str();
		}
	};

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
				<< "#define postcondition assert\n\n"
				<< toLoopFunctionHeader() << "\n"
				<< toLoopFunctionBody() << "\n"
				<< toLoopFunctionFooter() << "\n";
			//<< toMainFunction() << "\n";
			return stm.str();
		}

		std::string toReverseBranchFile4Klee(int b) {
			// used for selective sampling, from record_variable back to the program entrance...
			assert(b>=0 && b<loop->loopbody->n);
			//cout << "--------> toReverseBranchFile4Klee\n";
			std::ostringstream stm;
			stm << "#include \"candidate.h\"\n"
				<< toKleeInclude() << "\n"
				<< toKleeReverseExecutionMain(b) << "\n";
			//cout << "<-------- toReverseBranchFile4Klee\n";
			//<< toMainFunction() << "\n";
			return stm.str();
		}

		std::string toCFileWithLoopRecord(const char* cefile = "default.ce", const char* varfile = "default.var", const char* invfile = "default.inv", const char* pcfile = "default.pc") {
			// used for the previous setting, which we do not care about the intra-structure in the loop
			// only record the pass/fail of precondition and postcondition
			std::ostringstream stm;
			stm << toRecordInclude() << "\n"
				<< toLoopFunctionHeader() << "\n"
				<< toLoopFunctionBodyWithLoopRecord() << "\n"
				<< toLoopFunctionFooter() << "\n"
				<< toMainFunction(cefile, varfile, invfile, pcfile) << "\n";
			return stm.str();
		}

		std::string toCFileWithLoopBranchesRecord(const char* cefile = "default.ce", const char* varfile = "default.var", const char* invfile = "default.inv", const char* pcfile = "default.pc") {
			// used for our current setting
			// it can record branch info along with the pass/fail of precondition and postcondition
			std::ostringstream stm;
			stm << toRecordInclude() << "\n"
				<< toLoopFunctionHeader() << "\n"
				<< toLoopFunctionBodyWithLoopBranchesRecord() << "\n"
				<< toLoopFunctionFooter() << "\n"
				<< toMainFunction(cefile, varfile, invfile, pcfile) << "\n";
			return stm.str();
		}

		std::string toCFile4Verification(int choice) {
			// used for verifying the invariant candidate
			std::ostringstream stm;
			stm << toKleeInclude() << "\n"
				<< toKleeMain4Verification(choice);
			return stm.str();
		}
	private:
		std::string toInclude() {
			std::ostringstream stm;
			stm << "#include <iostream>\n" 
				<< "using namespace std;\n";
			return stm.str();
		}

		std::string toRecordInclude() {
			std::ostringstream stm;
			stm << "#include \"record.h\"\n"
				<< "#include <iostream>\n" 
				<< "using namespace std;\n"
				<< "using namespace record;\n";
			return stm.str();
		}

		std::string toKleeInclude() {
			std::ostringstream stm;
			stm << "#include <klee/klee.h>\n";
			return stm.str();
		}

		std::string toKleeMain4Verification(int choice) {
			std::ostringstream stm;
			stm << "int main() {\n"; 
			for (int i = 0; i < loop->names->n; i++) 
				stm << "\tint " + loop->names->u[i] + ";\n";
			for (int i = 0; i < loop->names->n; i++) 
				stm << "\tklee_make_symbolic(&" << loop->names->u[i] <<", sizeof(" << loop->names->u[i] << "), \"" << loop->names->u[i] << "\");\n";
			/*if (cs[SYM].value.compare("") != 0) {
			  stm << "int " + cs[SYM].value + ";\n";
			  stm << "klee_make_symbolic(&" << cs[SYM].value <<", sizeof(" << cs[SYM].value << "), \"" << cs[SYM].value << "\");\n";
			  } */
			// before loop statements;
			if (loop->beforeloop && loop->beforeloop->compare("") != 0)
				stm << "\t" << *loop->beforeloop << std::endl;

			//std::cout << "in toKleeMain4Verification with choice= " << choice << "... currently stm=\n@@" << stm.str() << "@@\n"; 
			switch (choice) {
				case 1:
					if (loop->beforeloopinit && loop->beforeloopinit->compare("") != 0)
						stm << *loop->beforeloopinit << std::endl;
					//std::cout << "222in toKleeMain4Verification... currently stm=\n@@" << stm.str() << "@@\n"; 
					stm << "\tklee_assume(" << *loop->precondition <<");\n"; 
					//std::cout << "333in toKleeMain4Verification... currently stm=\n@@" << stm.str() << "@@\n"; 
					stm << "\tklee_fail(" << *loop->loopinvariant <<");\n"; 
					//std::cout << "444in toKleeMain4Verification... currently stm=\n@@" << stm.str() << "@@\n"; 
					break;
				case 2:
					if (loop->beforeloopinit && loop->beforeloopinit->compare("") != 0)
						stm << "\t" << *loop->beforeloopinit << std::endl;
					if (loop->loopcondition && loop->loopcondition->compare("") != 0)
						stm << "\tklee_assume(" << *loop->loopcondition <<");\n"; 
					stm << "\tklee_assume(" << *loop->loopinvariant <<");\n"; 
					stm << "\tdo {\n";
					if (loop->loopbody->n > 0) {
						stm << "\t\t";
						for (int i = 0; i < loop->loopbody->n - 1; i++) {
							stm << "if (" << *loop->loopbody->branches[i].condition << ") {\n" 
								<< "\t\t\t" << *loop->loopbody->branches[i].body << "\n\t\t} else "; 
						}
						stm << "{\n" << "\t\t\t" << *loop->loopbody->branches[loop->loopbody->n - 1].body << "\n\t\t} \n"; 
					}
					//stm << "\t\t" << *loop->loopbody->body << "\n";
					stm << "\t} while(0);\n";
					stm << "\tklee_fail(" << *loop->loopinvariant <<");\n"; 
					break;
				case 3:
					if (loop->loopcondition && loop->loopcondition->compare("1") != 0) {
						if (loop->loopcondition && loop->loopcondition->compare("") != 0)
							stm << "\tklee_assume(!(" << *loop->loopcondition <<"));\n"; 
					} else if (loop->realloopcondition && loop->realloopcondition->compare("") != 0) {
						stm << "\tklee_assume(!(" << *loop->realloopcondition <<"));\n"; 
					}
					stm << "\tklee_assume(" << *loop->loopinvariant <<");\n"; 
					stm << "\tklee_fail(" << *loop->postcondition <<");\n"; 
					break;
			}
			stm << "\treturn 0;\n}" << endl;
			return stm.str();
		}

		std::string toKleeReverseExecutionMain(int b) {
			//cout << " -----> toKleeReverseExecutionMain(" << b << ")\n";
			std::ostringstream stm;
			stm << "int main() {\n"; 
			for (int i = 0; i < loop->names->n; i++) 
				stm << "\tint " + loop->names->u[i] + ";\n";
			for (int i = 0; i < loop->names->n; i++) 
				stm << "\tklee_make_symbolic(&" << loop->names->u[i] <<", sizeof(" << loop->names->u[i] << "), \"" << loop->names->u[i] << "\");\n";
			// before loop statements;
			if (loop->beforeloop && loop->beforeloop->compare("") != 0)
				stm << "\t" << *loop->beforeloop << std::endl;

			if (loop->beforeloopinit && loop->beforeloopinit->compare("") != 0)
				stm << "\t" << *loop->beforeloopinit << std::endl;
			if (loop->loopcondition && loop->loopcondition->compare("") != 0)
				stm << "\tklee_assume(" << *loop->loopcondition <<");\n"; 
			if (b == loop->loopbody->n - 1) {
				for (int i = 0; i < b; i++)
					stm << "\tklee_assume(!(" << *loop->loopbody->branches[i].condition <<"));\n"; 
			} else {
				stm << "\tklee_assume(" << *loop->loopbody->branches[b].condition <<");\n";
			}
			stm << "\tdo {\n";
			stm << "\t\t" << *loop->loopbody->branches[b].body << "\n";
			stm << "\t} while(0);\n";
			stm << "\tklee_pass(Candidate" << b << ");\n"; 
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
			if (loop->loopbody) {
				if (loop->loopbody->body) 
					stm << "\t\t" << *loop->loopbody->body << endl;
				if (loop->loopbody->n > 0) {
					stm << "\t\t";
					for (int i = 0; i < loop->loopbody->n - 1; i++) {
						stm << "if (" << *loop->loopbody->branches[i].condition << ") {\n" 
							<< "\t\t\t" << *loop->loopbody->branches[i].body << "\n\t\t} else "; 
					}
					stm << "{\n" << "\t\t\t" << *loop->loopbody->branches[loop->loopbody->n - 1].body << "\n\t\t} \n"; 
				}
			}
			stm << "\t}\n";
			stm << toAfterLoopBody();
			return stm.str();
		}

		std::string toRecordVariables() {
			std::ostringstream stm;
			stm << "record_variable_int(" << loop->names->u[0];
			for (int i = 1; i < loop->names->n; i++)
				stm << ", " << loop->names->u[i];
			stm << ");";
			return stm.str();
		}

		std::string toLoopFunctionBodyWithLoopRecord() {
			std::ostringstream stm;
			stm << toBeforeLoopBody();
			stm << "\t\t" << toRecordVariables() << endl;
			if (loop->loopbody) {
				if (loop->loopbody->body) 
					stm << "\t\t" << *loop->loopbody->body << endl;
				if (loop->loopbody->n > 0) {
					stm << "\t\t";
					for (int i = 0; i < loop->loopbody->n - 1; i++) {
						stm << "if (" << *loop->loopbody->branches[i].condition << ") {\n" 
							<< "\t\t\t" << *loop->loopbody->branches[i].body << "\n\t\t} else "; 
					}
					stm << "{\n" << "\t\t\t" << *loop->loopbody->branches[loop->loopbody->n - 1].body << "\n\t\t} \n"; 
				}
			}
			stm << "\t}\n";
			stm << "\t" << toRecordVariables() << endl;
			stm << toAfterLoopBody();
			return stm.str();
		}

		std::string toLoopFunctionBodyWithLoopBranchesRecord() {
			std::ostringstream stm;
			stm << toBeforeLoopBody();
			stm << "\t\t" << toRecordVariables() << endl;
			if (loop->loopbody) {
				if (loop->loopbody->body) 
					stm << "\t\t" << *loop->loopbody->body << endl;
				if (loop->loopbody->n > 0) {
					stm << "\t\t";
					for (int i = 0; i < loop->loopbody->n - 1; i++) {
						stm << "if (" << *loop->loopbody->branches[i].condition << ") {\n" 
							<< "\t\t\trecord_branch(" << i << ");\n" 
							<< "\t\t\t" << *loop->loopbody->branches[i].body << "\n\t\t} else "; 
					}
					stm << "{\n" << "\t\t\trecord_branch(" << loop->loopbody->n - 1 << ");\n" 
						<< "\t\t\t" << *loop->loopbody->branches[loop->loopbody->n - 1].body << "\n\t\t} \n"; 
				}
			}
			stm << "\t}\n";
			//stm << "\t" << toRecordVariables() << endl;
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
				stm << "\tprecondition(" << *loop->precondition << ");" << endl << endl;
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
				stm << "\n\tpostcondition(" << *loop->postcondition << ");" << endl << endl;
			stm << "\treturn 0;";
			return stm.str();
		}

		std::string toLoopFunctionFooter() {
			return string("}\n");
		}

		std::string toMainFunction(const char* cefile, const char* varfile, const char* invfile, const char* pcfile) {
			std::ostringstream stm;
			stm << "int main(int argc, char** argv) {\n";
			//stm << "\tContext context(\"../" << varfile <<"\", loopFunction, \"loopFunction\", \"../" << cefile << "\");\n";
			stm << "\tContext context(\"" << varfile <<"\", loopFunction, \"loopFunction\", NULL);\n";

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

			stm << "\treturn context.learn(\"" << cefile << "\", \"" << invfile << "\", \"" << pcfile << "\");" << endl;
			stm << "}";
			return stm.str();
		}
};

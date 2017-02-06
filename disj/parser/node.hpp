#ifndef _NODE_H_
#define _NODE_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

typedef enum {BLOOP, BLOOPI, PREC, LOOPC, RLOOPC, LOOP, BRANCHC, BRANCH, PSTC, ALOOP, INVR} tag_type; 
const std::string tag_name[] = {"beforeloop", "beforeloopinit", 
	"precondition", "loopcondition", "realloopcondition", "loopbody", "branchcondition", "branch",
	"postcondition", "afterloop", "invariant"};

#define BLANKS(n) do {for(int i = 0; i < (n); i++) cout << " ";} while(0)


class NameList{
	public:
		int n;
		std::string u[32];
		NameList() {n=0;}
		NameList* addName(std::string name) {
			if (n>=32)
				exit(-1);
			u[n] = name;
			n++;
			return this;
		}	

		void print (int start = 0) {
			for (int i = 0; i < n; i++) {
				BLANKS(start);
				cout << " |---" << u[i] << endl;
			}
		}
};

class Branch{
	public:
		std::string* condition;
		std::string* body;
		std::string* invariant;
		Branch() { }
		Branch(std::string* condition, std::string* body) {
			this->condition = condition;
			this->body = body;
			invariant = NULL;
		}
		Branch* setInvariant(std::string* invariant) {
			this->invariant = invariant;
			return this;
		}
};

class LoopBody {
	public:
		int n;
		std::string* body;
		Branch* branches;
		LoopBody() {
			n = 0;
			branches = new Branch[32];
		}
		LoopBody* addBranch(std::string* condition, std::string* body) {
			//branch[++n_branch] = new Branch(condition, body);
			branches[n].condition = condition;
			branches[n++].body = body;
			return this;
		}
		LoopBody* addBranchCondition(std::string* condition) {
			//cout << "->FUNCTION:: add branch condition (n=" << n << ")[" << *condition << "]\n";
			/*if (condition == NULL || *condition == "")
				branches[++n].condition = NULL;
			else
			*/
			branches[n].condition = condition;
			//cout << "<-FUNCTION:: add branch condition \n";
			return this;
		}
		LoopBody* addBranchBody(std::string* body) {
			branches[n++].body = body;
			return this;
		}
		void print(int start = 0) {
			for (int i = 0; i < n; i++) {
				BLANKS(start);
				cout << " |---[|" << (branches[i].condition? *branches[i].condition:"NULL") 
					<< "|] ---> [|" << (branches[i].body? *branches[i].body:"NULL") << "|]" <<endl;
			}
		}
};


class Loop{
	public:
		NameList* names;
		std::string* beforeloop;
		std::string* beforeloopinit;
		std::string* precondition;
		std::string* loopcondition;
		std::string* realloopcondition;
		LoopBody* loopbody;
		std::string* postcondition;
		std::string* afterloop;
		std::string* loopinvariant;
		NameList* learners;
		Loop() {
			loopbody = new LoopBody();
		}
		
		// to be compatible with the old ZILU
		void adjust() {
			if (loopbody->body != NULL && loopbody->n == 0) {
				//loopbody->n = 1;
				addItem(BRANCH, loopbody->body);
				loopbody->body = NULL;
			}
		}

		void print(int start = 0) {
			cout << "Loop-------------------------------------\n";
			cout << " |---[namelist]---------------" << endl;
			names->print(5);
			BLANKS(start); 
			cout << " |---" << "[beforeloop] [|" << (beforeloop? *beforeloop:"NULL") << "|]" << endl;
			BLANKS(start); 
			cout << " |---" << "[beforeloopinit] [|" << (beforeloopinit? *beforeloopinit:"NULL") << "|]" << endl;
			BLANKS(start); 
			cout << " |---" << "[precondition] [|" << (precondition? *precondition:"NULL") << "|]" << endl;
			BLANKS(start); 
			cout << " |---" << "[loopcondition] [|" << (loopcondition? *loopcondition:"NULL") << "|]" << endl;
			BLANKS(start); 
			cout << " |---" << "[realloopcondition] [|" << (realloopcondition? *realloopcondition:"NULL") << "|]" << endl;
			BLANKS(start); 
			cout << " |---" << "[loopbody] [|" << (loopbody->body? *loopbody->body:"NULL") <<  "|]" << endl;
			loopbody->print(start + 7);
			BLANKS(start); 
			cout << " |---" << "[postcondition] [|" << (postcondition? *postcondition:"NULL") << "|]" << endl;
			BLANKS(start); 
			cout << " |---" << "[afterloop] [|" << (afterloop? *afterloop:"NULL") << "|]" << endl;
			BLANKS(start); 
			cout << " |---" << "[loopinvariant] [|" << (loopinvariant? *loopinvariant:"NULL") << "|]" << endl;
			if (learners != NULL) {
				cout << " |---[learnerlist]---------------" << endl;
				learners->print(5);
				cout << endl;
			}
			//cout << " |---" << endl;
		}

		Loop* addItem(tag_type type, std::string* pstr) {
			//cout << ">>>" << type << " ---------- " << *str << endl;
			switch(type) {
				case BLOOP:
					beforeloop = pstr;
					break;
				case BLOOPI:
					beforeloopinit = pstr;
					break;
				case PREC:
					precondition = pstr;
					break;
				case LOOPC:
					loopcondition = pstr;
					break;
				case RLOOPC:
					realloopcondition = pstr;
					break;
				case LOOP:
					loopbody->body = pstr;
					break;
				case BRANCHC:
					loopbody->addBranchCondition(pstr);
					break;
				case BRANCH:
					loopbody->addBranchBody(pstr);
					break;
				case PSTC:
					postcondition = pstr;
					break;
				case ALOOP:
					afterloop = pstr;
					break;
				case INVR:
					loopinvariant = pstr;
					break;
				default:
					cerr << "Should not reach here.\n";
			}
			return this;
		}
};


#endif

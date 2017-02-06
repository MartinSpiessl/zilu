#ifndef _NODE_H_
#define _NODE_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

typedef enum {BLOOP, BLOOPI, PREC, LOOPC, LOOP, PSTC, ALOOP, INVR} tag_type; 
const std::string tag_name[] = {"beforeloop", "beforeloopinit", 
	"precondition", "loopcondition", "loopbody", "postcondition", "afterloop", "invariant"};

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

class Loop{
	public:
		NameList* names;
		std::string* beforeloop;
		std::string* beforeloopinit;
		std::string* precondition;
		std::string* loopcondition;
		std::string* loopbody;
		std::string* postcondition;
		std::string* afterloop;
		std::string* loopinvariant;
		NameList* learners;
		Loop() {}
		
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
			cout << " |---" << "[loopbody] [|" << (loopbody? *loopbody:"NULL") <<  "|]" << endl;
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
				case LOOP:
					loopbody = pstr;
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

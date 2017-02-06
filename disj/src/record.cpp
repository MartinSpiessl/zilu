#include "record.h"
#include "base_learner.h"
#include "linear_learner.h"
#include "poly_learner.h"
#include "conjunctive_learner.h"
using namespace record;

int iifround = 0;
int initseed = 0;

LearnerNode::LearnerNode() {
	learner = NULL;
	next = NULL;
}
LearnerNode::~LearnerNode() {
	if (learner != NULL)
		delete learner;
}

/*Context::Context (States* ss) {
  branch_done = new bool[Nb];
  for (int i = 0; i < Nb; i++)
  branch_done[i] = false;
  branch_invariants = new std::string[Nb];
  for (int i = 0; i < Nb; i++)
  branch_invariants[i] = "unknown";
  gsets = ss;
  first = NULL;
  last = NULL;
  variables = NULL;
  vnum = 0;
  }*/

Context::Context(const char* filename_var, int (*func)(int*), 
		const char* func_name, const char* dataset_fname, int timeout) {
	branch_done = new bool[Nb];
	for (int i = 0; i < Nb; i++)
		branch_done[i] = false;
	branch_invariants = new std::string[Nb];
	for (int i = 0; i < Nb; i++)
		branch_invariants[i] = "unknown";
	std::ifstream varFile(filename_var);
	varFile >> vnum;
	variables = new std::string[Cv0to4];
	vparray = new VariablePowerArray[Cv0to4];
	variables[0] = '1';
	for (int i = 0; i < Cv0to4; i++) {
		for(int j = 0; j < Nv; j++) {
			vparray[i][j] = 0;
		}
	}
	for (int i = 1; i <= Nv; i++) {
		varFile >> variables[i];
		vparray[i][i-1] = 1;
	}
	varFile.close();
	int index = Nv+1;
	for (int i = 0; i < Nv; i++) {
		for (int j = i; j < Nv; j++) {
			vparray[index][i]++;
			vparray[index][j]++;
			variables[index++] = /*"(" +*/ variables[i+1] + "*" + variables[j+1] /*+ ")"*/;
		}
	}
	for (int i = 0; i < Nv; i++) {
		for (int j = i; j < Nv; j++) {
			for (int k = j; k < Nv; k++) {
				vparray[index][i]++;
				vparray[index][j]++;
				vparray[index][k]++;
				variables[index++] = /*"(" +*/ variables[i+1] + "*" + variables[j+1] 
					+ "*" + variables[k+1] /*+ ")"*/;
			}
		}
	}
	for (int i = 0; i < Nv; i++) {
		for (int j = i; j < Nv; j++) {
			for (int k = j; k < Nv; k++) {
				for (int l = k; l < Nv; l++) {
					vparray[index][i]++;
					vparray[index][j]++;
					vparray[index][k]++;
					vparray[index][l]++;
					variables[index++] = /*"(" +*/ variables[i+1] + "*" + variables[j+1] 
						+ "*" + variables[k+1] + "*" + variables[l+1]/* + ")"*/;
				}
			}
		}
	}

	States* ss = new States[3];
	gsets = &ss[1];
	//std::cout << YELLOW << "$$$$$$$$$$$$$$$$$$$$$$$$<gsets=" << gsets << ">, ss=<" << ss << ">" <<  NORMAL;
	//std::cout << gsets[-1];
	gsets[NEGATIVE].label = NEGATIVE;
	gsets[QUESTION].label = QUESTION;
	gsets[POSITIVE].label = POSITIVE;
	//gsets[CNT_EMPL].label = CNT_EMPL;
	if (dataset_fname != NULL) {
		std::cout << "dataset filename := " << dataset_fname << std::endl;
		std::ifstream fin(dataset_fname);
		if (fin) {
			int l, pn, nn;
			fin >> l >> pn >> nn;
			std::cout << "l:= " << l << "  pn:=" << pn << "  nn:=" << nn << std::endl;
			gsets[POSITIVE].initFromFile(pn, fin);
			gsets[NEGATIVE].initFromFile(nn, fin);
			fin.close();
		}
	}
	first = NULL;
	last = NULL;
	register_program(func, func_name);
	this->timeout = timeout;
	srand(time(NULL)); // initialize seed for rand() function
	//srand(initseed); // initialize seed for rand() function
}

Context::~Context() {
	// clean up heap memory
	if (branch_done) {
		delete []branch_done;
		branch_done = NULL;
	}
	if (branch_invariants) {
		delete []branch_invariants;
		branch_invariants = NULL;
	}
	LearnerNode* p = first;
	LearnerNode* pp;

	while (p != NULL) {
		pp = p->next;
		delete p;
		p = pp;
	}

	/*if (gsets) {
		delete []&gsets[-1];
		gsets = NULL;
	}*/

	if (gsets) {
		States* ss = &gsets[-1];
		delete []ss;
		ss = NULL;
		gsets = NULL;
	}

	if (variables != NULL) {
		delete []variables;
		variables = NULL;
	}
	if (vparray != NULL) {
		delete vparray;
		vparray = NULL;
	}
}


Context& Context::addLearner(const char* learnerName) {
	BaseLearner* newLearner = NULL;
	if (strcmp(learnerName, "linear") == 0)
		newLearner = new LinearLearner(gsets);
	else if (strcmp(learnerName, "poly") == 0)
		newLearner = new PolyLearner(gsets);
	else if (strcmp(learnerName, "conjunctive") == 0)
		newLearner = new ConjunctiveLearner(gsets);

	if (last == NULL) {
		last = new LearnerNode();
		first = last;
	} else {
		last->next = new LearnerNode();
		last = last->next;
	}
	last->learner = newLearner;
	return *this;
}

int Context::learn(const char* filename_cnt, const char* filename_inv, const char* filename_pathc, int times) {
	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		exit(-1);
	alarm(timeout);

	if (first == NULL) {
		std::cerr << "Leaner has not been assigned. Please add 'learners=***' in your cfg file.\n";
		return 1;
	}

	std::string pathc[Nb];
	std::ifstream f_pc(filename_pathc);
	std::string tmp_pc;
	for (int b = 0; b < Nb; b++) {
		f_pc >> tmp_pc;
		pathc[b] = "(" + tmp_pc + ")";
	}
	f_pc.close();


	//*
	for (int iverify = 1; iverify <= Mverify; iverify++) {
		std::cout << YELLOW << BOLD << "######################################## " << iverify <<" #############################################\n" << NORMAL;
		iifround = iverify;
		if (filename_cnt) {
			first->learner->runCounterExampleFile(filename_cnt);
			//BaseLearne::runCounterExampleFile(filename_cnt);
		}
		//return 0;

		for (int b = 0; b < Nb; b++) {
			if (branch_done[b] == false) {
				/*std::cout << "\n" << NORMAL;
				for (int i=0; i<12; i++)
					std::cout << BOLD << GREEN << "*********Branch " << b << "***";
				std::cout << "*\n" << NORMAL;
				*/
				std::cout << "\n" << BOLD << GREEN << "*********Branch " << b << "********\n" << NORMAL;
				LearnerNode* p = first;
				while (p) {
					if (p->learner->learn(b) == 0) {
						branch_invariants[b] = "((" + p->learner->invariant() + ") && " + pathc[b] + ")  ";
						branch_done[b] = true;
						break;
					} else {
						p = p->next;
					}
				}
				//std::cout << GREEN << BOLD << "branch[" << b << "] learning complete ---> " << branch_invariants[b] << "\n" << NORMAL;
			}
		}
		//*/

		// the invariants for all the paths are learnt completely.
		//std::cout << RED << "After all the branches learrning, watch point at line#187...\n" << NORMAL;
		for (int b = 0; b < Nb; b++) {
			std::cout << BLUE << "/*b" << b << "==>*/ " << GREEN << "(" << branch_invariants[b] << ")";
			if (b < Nb-1)
				std::cout << " || ";
		}
		std::cout << "\n" << NORMAL;

		for (int b = 0; b < Nb; b++) {
			if (branch_done[b] == false) {
				return 1;
			}
		}

		std::ofstream invFile(filename_inv);
		for (int b = 0; b < Nb; b++) {
			invFile << "(" << branch_invariants[b] << ")";
			if (b < Nb-1)
				invFile << " || ";
		}
		invFile.close();

		//if(External::candidateVerify(32) == 0) {
		if(External::candidateVerify(1) == 0) {
			std::cout << YELLOW << BOLD << "VERIFIED." << NORMAL;
			return 0;
		}
		for (int b = 0; b < Nb; b++) {
			branch_done[b] = false;
		}
	}
#if 0
	char filename[256]; 
#ifdef __DS_ENABLED
	sprintf(filename, "%s.ds", (char*)filename_inv);
	p->learner->save2file(filename);
#endif
#endif
	return 1;
}


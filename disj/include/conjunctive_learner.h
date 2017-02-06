#ifndef _CONJUNCTIVE_
#define _CONJUNCTIVE_
#include "config.h"
#include "base_learner.h"
#include "svm_i.h"


class ConjunctiveLearner: public BaseLearner {
	public:
		ConjunctiveLearner(States* gsets, /*const char* solution_filename = NULL,*/ int (*func)(int*) = target_program, int max_iteration = Miter);

		~ConjunctiveLearner();

		virtual int save2file(const char*);
		
		virtual int learn(int branch = 0);

		virtual std::string invariant();

	protected:
		SVM_I* svm_i;
		int max_iteration;
};

#endif

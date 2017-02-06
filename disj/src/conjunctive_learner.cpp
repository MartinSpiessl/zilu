//#include "config.h"
#include "ml_algo.h"
#include "svm.h"
//#include "color.h"
#include "polynomial.h"
#include "conjunctive_learner.h"

#include <iostream>
#include <float.h>
#include <string.h>
#include <assert.h>

static void print_null(const char *s) {}

ConjunctiveLearner::ConjunctiveLearner(States* gsets, int (*func)(int*), int max_iteration) 
	: BaseLearner(gsets, func) { 
		svm_i = new SVM_I(0, print_null);
		this->max_iteration = max_iteration;
	}

ConjunctiveLearner::~ConjunctiveLearner() { 
	if (svm_i != NULL)
		delete svm_i;
}

int ConjunctiveLearner::learn(int branch)
{
	std::cout << YELLOW << ">>>> Conjunctive Learner-----------------------\n" << NORMAL;  
	Solution inputs;
	srand(time(NULL)); // initialize seed for rand() function

	int rnd;
	//bool lastSimilar = false;
	bool converged = false;
	int converged_time = 0;
	Classifier pre_cl;
	int pre_psize = 0, pre_nsize = 0; // , pre_question_size = 0;
	double pass_rate = 1;

	for (rnd = 1; ((rnd <= max_iteration) && (pass_rate >= 1)); rnd++) {
		int zero_times = 0;
		//std::cout << "[" << rnd << "]";
		int nexe = (rnd == 1) ? Nexe_init : Nexe_after;
#ifdef __PRT
		int step = 1;
		std::cout << RED << "[" << rnd << "]" << NORMAL;
		std::cout << "SVM-I----------------------------------------------------------"
			"------------------------------------------------";
		std::cout << "\n\t(" << step++ << ") execute programs... [" << nexe + Nexe_rand << "] ";
#else
		std::cout << RED << "[" << rnd;
#endif
init_svm_i:
		selectiveSampling(Nexe_rand, nexe, &pre_cl, branch);

		std::cout << BOLD << YELLOW << "Positive trace [" << gsets[POSITIVE].traces_num() <<"] Negative trace [" << gsets[NEGATIVE].traces_num() << "]." << NORMAL << std::endl;
		if ((rnd == 1) && (gsets[POSITIVE].traces_num() == 0 || gsets[NEGATIVE].traces_num() == 0)) {
#ifdef __PRT
			if (gsets[POSITIVE].traces_num() == 0) 
				std::cout << RED << "\tZero Positive trace, execute program again." << NORMAL << std::endl;
			if (gsets[NEGATIVE].traces_num() == 0) 
				std::cout << RED << "\tZero Negative trace, execute program again." << NORMAL << std::endl;
#else
			std::cout << "+";
#endif
			if (++zero_times >= Nretry_init) {
				if (gsets[POSITIVE].traces_num() == 0) 
					std::cout << RED << "Can not get any positive trace. " << std::endl;
				else
					std::cout << RED << "Can not get any negative trace. " << std::endl;
				std::cout << " re-Run the system again OR modify your loop program.\n" << NORMAL;
				exit(-1);
			}
			if (maxv <= 100000) {maxv+=base_step;}
			if (minv >= -100000) {minv-=base_step;}
			goto init_svm_i;
		}

		svm_i->mapData(gsets, pre_psize, pre_nsize);
#ifdef __PRT
		std::cout << "\t(" << step++ << ") prepare training data... ";
#else
		if (zero_times == 0) {
			std::cout << "]" << NORMAL;
		}
#endif
#ifdef __PRT_STATISTICS
		std::cout << "[#r" << random_samples << ",#s" << selective_samples << "]\n    ";
#endif
		std::cout << "total positive=" << gsets[POSITIVE].size;
		for (int i=0; i < Nb; i++)
			std::cout << " |---[branch" << i << "]=" << GREEN << gsets[POSITIVE].size_b[i] << NORMAL;

#ifdef __PRT
		std::cout << "\n\t(" << step++ << ") start training... ";
#endif
		//if (svm_i->makeTrainingSet(gsets, pre_psize, pre_nsize) == 0) {
		if (svm_i->makeTrainingSet(branch) == 0) {
			if (++zero_times < Nretry_init)
				goto init_svm_i;
		}
		svm_i->setBranch(branch);
		//while (pre_psize + pre_nsize >= density * pow(maxv-minv, Nv)) {
		while (gsets[POSITIVE].size + gsets[NEGATIVE].size >= density * pow(maxv-minv, Nv)) {
			if (maxv <= 100000) {maxv+=base_step;}
			if (minv >= -100000) {minv-=base_step;}
		}

#ifdef __DS_ENABLED
		std::cout << "[" << svm_i->problem.np << "+:" << svm_i->negative_size << "-]";
#endif
		int ret = svm_i->train();
		if (ret == -1) {
			std::cerr << RED << "[FAIL] ..... Can not divided by SVM_I." << std::endl << NORMAL;
			return -1;
		}
		//svm_i->cl.roundoff();
		std::cout << "|-->> " << YELLOW << *svm_i << std::endl << NORMAL;

		/*
		 *	check on its own training data.
		 *	There should be no prediction errors.
		 */
#ifdef __PRT
		std::cout << "\t(" << YELLOW << step++ << NORMAL << ") checking training traces.";
#endif
		pass_rate = svm_i->checkTrainingSet();

#ifdef __PRT
		if (pass_rate == 1) 
			std::cout << GREEN << " [" << pass_rate * 100 << "%]" << NORMAL;
		else 
			std::cout << RED << " [" << pass_rate * 100 << "%]" << NORMAL;
#endif

		if (pass_rate < 1) {
			std::cerr << RED << "[FAIL] ..... Can not divided by SVM_I." << std::endl << NORMAL;
			rnd++;
			break;	
		}
#ifdef __PRT
		std::cout << GREEN << " [PASS]" << std::endl << NORMAL;
#endif

#ifndef __SELECTIVE_SAMPLING_ENABLED
		converged = true;
		break;
#endif

#ifdef __QUESTION_TRACE_CHECK_ENABLED
#ifdef __PRT
		std::cout << "\t(" << YELLOW << step++ << NORMAL << ") check Question Traces:   ";
#endif
		if (svm_i->checkQuestionTraces(gsets[QUESTION]) != 0)
			continue;
#ifdef __PRT
		std::cout << "\n";
#endif
#endif
		svm_i->pointwiseSimplify();

		/*
		 *	lastSimilar is used to store the convergence check return value for the last time.
		 *	We only admit convergence if the three consecutive round are converged.
		 *	This is to prevent in some round the points are too right to adjust the classifier.
		 */
#ifdef __PRT
		std::cout << "\t(" << YELLOW << step++ << NORMAL << ") check convergence:        ";
#endif

		if (svm_i->converged(pre_cl) == true) {
			converged_time++;
#ifdef __PRT
			std::cout << "[";
			for (int j = 0; j < converged_std - converged_time; j++)
				std::cout << "F";
			for (int j = 0; j < converged_time; j++)
				std::cout << "T";
			std::cout << "]  ";
#endif

			if (converged_time >= converged_std) {
#ifdef __PRT
				std::cout << "[SUCCESS] rounding off" << std::endl;
#endif
				converged = true;
				rnd++;
				break;
			}
		} else {
			converged_time = 0;
		}
#ifdef __PRT
		std::cout << "[FAIL] neXt round " << std::endl;
#endif

		pre_cl = svm_i->cl;
		svm_i->cl.clear();
	} // end of SVM_I training procedure


	std::cout << "---------------------------------------------------\n";
	std::cout << "Finish running svm_i for " << rnd - 1 << " times." << std::endl;

	int ret = 0;
	if ((converged) && (rnd <= max_iteration)) {
		//svm_i->pointwiseSimplify();
		svm_i->cl.roundoff();
		svm_i->cl.simplify();
		std::cout << YELLOW << "  Hypothesis Invairant(Conjunctive): { ";
		std::cout << GREEN << svm_i->cl << YELLOW;
		std::cout << " }" << NORMAL << std::endl;
	}

	if ((pass_rate < 1) || (rnd > max_iteration)) {
		//std::cout << RED << "  Cannot divided by SVM_I perfectly.\n" << NORMAL;
		std::cout << pass_rate << "\t" << rnd << std::endl;
		ret = -1;
	}

	//std::cout << "return value = " << ret << std::endl;
	return ret;
}


std::string ConjunctiveLearner::invariant() {
	svm_i->cl.roundoff();
	return svm_i->cl.toString();
}

int ConjunctiveLearner::save2file(const char* dsfilename) {
	printStatistics();
	//std::ofstream fout("../tmp/svm.ds");
	std::ofstream fout(dsfilename);
	fout << svm_i->problem.np + svm_i->negative_size  << "\t" << svm_i->problem.np << "\t" << svm_i->negative_size << "\n";
	for (int i = 0; i < svm_i->problem.np; i++) {
		fout << 1;
		for (int j = 0; j < Nv; j++)
			fout << "\t" << j << ":" << (svm_i->problem.x[i][j]).value;
		fout << "\n";
	}
	for (int i = 0; i < svm_i->negative_size; i++) {
		fout << -1;
		for (int j = 0; j < Nv; j++)
			fout << "\t" << j << ":" << svm_i->negative_mapped_data[i][j];
		fout << "\n";
	}
	fout.close();
	//std::cout << "save to file succeed. ../tmp/svm.ds\n";
	std::cout << "save the training dataset to file " << dsfilename << "\n";
	return 0;
}

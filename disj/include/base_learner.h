/** @file iif_learn.h
 *  @brief Provide base class for all iif learning algorithms.
 *
 *  This file contains the necessary function support for iif learning algorithms.
 *  The function run_target is the most important function in this base class for now. 
 *
 *  @author Li Jiaying
 *  @bug no known bugs found.
 */
#ifndef _LEARNER_BASE_
#define _LEARNER_BASE_

#include "config.h"
#include "states.h"
#include "polynomial.h"
#include "classifier.h"
//#include "candidate.h"
#include "instrumentation.h"
#include "color.h"
#include "external.h"
//#include "record.h"

#include <iostream>
#include <iomanip>
#include <float.h>
#include <string.h>
#include <sstream>
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef __PRT_STATISTICS
extern int random_samples, selective_samples;
#endif
extern int iifround;


class BaseLearner{
	public:
		BaseLearner(States* gsets, /*const char* cntempl_fname = NULL,*/ int (*func)(int*) = target_program):
			gsets(gsets), func(func) { }

		virtual ~BaseLearner() {} 

		void runCounterExampleFile(const char* cntempl_fname = NULL) {
			std::cout << RED << ">>>> run counter example from file %%" << cntempl_fname << NORMAL << std::endl;
			std::cout.unsetf(std::ios::fixed);
			if (cntempl_fname!= NULL) {
				std::ifstream fin(cntempl_fname);
				//std::cout << RED << "Running Counter-Examples from file[" << cntempl_fname << "]\n" << NORMAL;
				if (fin) {
					Solution s;
					while (fin >> s) {
						std::cout << BLUE << BOLD << "Test Counter Example: "
							<< s << " @" << cntempl_fname << " --> " << NORMAL;
#ifdef __PRT_TESTCASE
						std::cout << BLUE << "  |---> " ;
						std::cout << s;
						std::cout << " ->> " << NORMAL;
#endif
						//int ret = runTarget(s);
						//runTarget(s);
						int ret = runTarget(s);
						printRunResult(ret);
						std::cout << std::endl << NORMAL;
#ifdef __PRT_TRACE
						//std::cout << "\n";
#endif
					}
					int newscope = maxv;
					for (int i = 0; i < Nv; i++) {
						while(std::abs(s[i]) > newscope) {
							if (newscope * 2 >= 0)
								newscope *= 2;
							else 
								break;
						}
					}
					if (newscope > maxv) {
						maxv = newscope;
						minv = -1 * maxv;
#ifdef __PRT
						std::cout << YELLOW << "new scope:=[" << minv << "," << maxv << "]" << NORMAL << std::endl;
#endif
					}
					fin.close();
				}
			}
		}

		virtual int save2file(const char*) = 0;
		/** @brief This function runs the target_program with the given input
		 *
		 *  @param  input defines input values which are used to call target_program 
		 */
		int runTarget(Solution& input) {
			assert(func != NULL || "Func equals NULL, ERROR!\n");
			beforeLoop();

			//< convert the given input with double type to the input with int type 
			int a[Nv];
			for (int i = 0; i < Nv; i++)
				a[i] = static_cast<int>(input[i]);
			//target_program
			//std::cout << "----> run the loop function.\n";
			func(a);
			//std::cout << "<---- run the loop function.\n";

			int label = afterLoop(gsets);
 			//std::cout << "<---- after afterLoop function.\n";
			if (label == CNT_EMPL) {
				std::cout << RED << BOLD << " \nBUG! Program encountered a Counter-Example trace." << std::endl;
				exit(-2);
			}
			//std::cout << "here105.\n";
			return label;
		}

		/** @brief This method is the entrance for the whole learning procedure.
		 *		   Child class should implement it based on learning algorithm.
		 */
		virtual int learn(int branch = 0) = 0;

		/** @brief This method is used to generate new input and drive the testing process.
		 *		   This method is actually does several jobs, depend on parameters. 
		 *		   It is better to split it into several methods.
		 *
		 *		   You can find details of each parameters in child class.
		 */
		int selectiveSampling(int randn, int exen, Classifier* cl, int branch = 0) {
#ifdef __PRT
			std::cout << "{" << GREEN;
#endif
#ifdef __PRT_TESTCASE
			std::cout << "\n" << GREEN;
#endif

#ifndef __SELECTIVE_SAMPLING_ENABLED
			std::cout << "Pure Random";
			randn += exen;
			exen = 0;
#endif
			if (iifround >= 2) {
				randn = 0;
				//std::cout << "No more samples added.\n";
			}
			Solution input;
			int ret = 0;
			for (int i = 0; i < randn; i++) {
				Classifier::solver(NULL, input);
#ifdef __PRT_STATISTICS
				random_samples++;
#endif
#ifdef __PRT
				//std::cout << "-------> Testing on input: " << input << std::endl;
				std::cout << input << " ";
#endif
				ret = runTarget(input);
#ifdef __PRT_TRACE
				std::cout << "\n";
#endif
#ifdef __PRT
				//std::cout << "<------- Testing Done: " << ret << std::endl;
				/*std::cout << input << endl;
				  printRunResult(ret);
				  std::cout << "|";
				  */
#endif
			}

			Solution* sols = new Solution[exen];
			int num = External::selectiveSampling(branch, cl->toString(), exen, sols);
			//int num = externalSelectiveSampling(0, cl->toString(), exen, sols);
			//std::cout << YELLOW << "back to selective sampling function.\n";
#ifdef __PRT_TESTCASE
			std::cout << BLUE << "get #" << num << " samples: ";
			for (int i = 0; i < num; i++) {
				std::cout << "[" << sols[i] << "] ";
			}
			std::cout << std::endl;
#endif
#ifdef __PRT
			std::cout << BLUE;
#endif

			for (int i = 0; i < num; i++) {
#ifdef __PRT_STATISTICS
				selective_samples++;
#endif
#ifdef __PRT
				std::cout << sols[i] << " ";
				//std::cout << "-------> Testing on input: " << sols[i]<< std::endl;
#endif
				ret = runTarget(sols[i]);
#ifdef __PRT_TRACE
				std::cout << "\n";
#endif
#ifdef __PRT
				//std::cout << "<------- Testing Done: " << ret << std::endl;
				/*std::cout << "|" << input;
				  printRunResult(ret);
				  */
#endif
			}

#ifdef __PRT
			std::cout << NORMAL << "}" << std::endl;
#endif
			delete []sols;
			return randn + exen;
		}

		/** @brief This method try to return a readable string which describe the generated invariant.
		 *		   The returned method is also required to be a valid expression which can be used in C program,
		 *		   because we will directly insert the string to the program to be verified.
		 */
		virtual std::string invariant() = 0;

		void printStatistics() {
#ifdef __PRT_STATISTICS
			//std::cout << GREEN << BOLD << "***********************STATISTICS*********************\n";
			//std::cout << GREEN << BOLD << "|*\t\t   " << RED << "random_samples= " << random_samples << "\n";
			//std::cout << GREEN << BOLD << "|*\t\t   " << RED << "selective_samples= " << selective_samples << "\n";
			//std::cout << GREEN << BOLD << "******************************************************\n" << NORMAL;
			std::ofstream of1("../result/statistics", std::ofstream::app);
			struct timeval tv;
			gettimeofday(&tv, NULL);
			//time_t nowtime = tv.tv_sec;
			struct tm* nowtm = localtime(&tv.tv_sec);
			char tmbuf[64], buf[64];
			//strftime(tmbuf, sizeof(tmbuf), "%Y-%m-%d %H:%M:%S", nowtm);
			strftime(tmbuf, sizeof(tmbuf), "%H:%M:%S", nowtm);
			snprintf(buf, sizeof(buf), "%s.%06ld", tmbuf, tv.tv_usec);
			//of1 << buf << "\t\t" << random_samples << "\t\t" << selective_samples << std::endl;
			of1 << random_samples << "\t" << selective_samples << std::endl;
			//of1 << "\t\t#r_samples=" << random_samples << "\t\t#s_samples=" << selective_samples << std::endl;
			of1.close();
#endif
			std::cout<< "\t\t#r_samples=" << random_samples << "\t\t#s_samples=" << selective_samples << "\t TOTAL:" << BOLD << GREEN << random_samples + selective_samples << NORMAL << std::endl;
		}

	protected:
		States* gsets;
		int (*func)(int*);
};


#endif

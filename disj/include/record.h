/** @file record.h
 *  @brief Contains all the files that needed to be included by a new test.
 *  
 *  By include this file, it should resolve all the reference errors to the framework.
 *
 *  @author Li Jiaying
 *  @bug No found bugs
 */
#ifndef _RECORD_H_
#define _RECORD_H_

#include "config.h"
#include "color.h"
#include "macro.h"
#include "base_learner.h"

#include <iostream>
#include <float.h>
#include <string>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>


extern int minv, maxv;
extern std::string* variables;
extern int vnum;
extern VariablePowerArray* vparray;
extern int iifround;
extern int initseed;


namespace record{
	class LearnerNode {
		public:
			LearnerNode();
			~LearnerNode();
			BaseLearner* learner;
			LearnerNode* next;
	};

	class Context {
		private:
			static void sig_alrm(int signo) {
				std::cout << "\nTIMEOUT!\n";
				exit(-1);
			}

		public:
			//Context (States* ss);

			Context (const char* filename_var, int (*func)(int*), const char* func_name = "Unknown", const char* filename_previoustestcase = NULL, int timeout = 3600);

			~Context();

			Context& addLearner(const char* learnerName);

			int learn(const char* filename_cnt = NULL, const char* filename_inv = "noname", const char* filename_pathc = "noname.pc", int times = 1);

		private:
			bool* branch_done;
			std::string* branch_invariants;
			States* gsets;
			LearnerNode* first;
			LearnerNode* last; 
			int timeout;
	};

}


#endif

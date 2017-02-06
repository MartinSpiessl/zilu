/** @file iif_learn.h
 *  @brief Provide base class for all iif learning algorithms.
 *
 *  This file contains the necessary function support for iif learning algorithms.
 *  The function run_target is the most important function in this base class for now. 
 *
 *  @author Li Jiaying
 *  @bug no known bugs found.
 */
#ifndef _EXTERNAL_
#define _EXTERNAL_

#include "config.h"
#include "states.h"
#include "polynomial.h"
#include "classifier.h"
#include "instrumentation.h"
#include "color.h"

#include <iostream>
#include <iomanip>
#include <float.h>
#include <string.h>
#include <sstream>
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>

class External{
	public:
		static int selectiveSampling(int branch, std::string candidate, int num, Solution* sols) {
			//std::cout <<  RED << BOLD << branch << "\t" << candidate << std::endl << NORMAL;
			std::ofstream ofs(ProjectHome"/tmp/tmp.cand");
			ofs << branch << "\t" << candidate << std::endl;
			ofs.close();
			std::ostringstream stm;
			stm << ProjectHome"/scripts/ssampling.sh " << Project << " " << ProjectHome"/tmp/tmp.cand " << num;
			//system("cd "ProjectHome);
			
			if (system(stm.str().c_str()) != 0) {
				std::cerr << "Error occured in script/ssampling.sh\n";
				exit(1);
			}
			//system("rm -f "ProjectHome"/tmp/tmp.cand");
			//std::cout << "ssampling.sh completed...\n";
			std::ifstream ifs(ProjectHome"/tmp/"Project".cnt");
			//if (ret <= num + 2)
			//	ret = num - 2;
			int i = 0;
			while (ifs.good() && i < num) {
				for (int j = 0; j < Nv; j++) {
					ifs >> sols[i][j];
				}
				i++;
			}
			int ret = i;
			
			ifs.close();
			//std::cout << "<------return from externalSelectiveSampling...\n";
			return ret;
		}

		static int candidateVerify(int cnt_num = 8) {
			std::ostringstream stm;
			stm << ProjectHome"/scripts/verify.sh " << Project << " " << cnt_num;
			int ret = system(stm.str().c_str());
			if (ret != 0) {	
				std::cerr << RED << BOLD << "Candidate can not be verified.\n" << NORMAL;
			}
			return ret;
		}
};

#endif

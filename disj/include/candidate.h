/** @file classifier.h
 *
 *  @author Li Jiaying
 *  @bug no known bugs found.
 */
#ifndef _CANDIDATES_
#define _CANDIDATES_

#if 0
#include "config.h"
#include "states.h"
#include "polynomial.h"
#include "classifier.h"
#include "color.h"

#include <iostream>
#include <float.h>
#include <string.h>
#include <assert.h>

class Polynomial;

class Candidate{
	protected:
		int size;
		//std::vector<Classifier*> cls;
		Classifier* cls;

	public:
		Candidate() {
			cls = new Classifier[Nb];
		}
		
		~Candidate() { 
			delete []cls;
		} 

		int getSize() const {
			return Nb;
		}

		int add(int b, Classifier* cl) {
			assert(b>=0 && b<Nb);
			cls[b] = *cl;
			return 0;
		}

		int clear();

		std::string toString() const {
			std::ostringstream stm;
			stm << "(" << cls[0] << ")";
			for (int i = 1; i < Nb; i++) {
				stm << " || (" << cls[i] << ")";
			}
			return stm.str();
		};

		Classifier* operator[] (int index) {
			if (index>=0 && index<Nb)
				return &cls[index];
			return NULL;
		}

		Classifier* operator[] (int index) const {
			if (index>=0 && index<Nb)
				return &cls[index];
			return NULL;
		}

	friend std::ostream& operator << (std::ostream& out, const Candidate& cs) {
		out << cs.toString();
		return out;
	}
};

#endif
#endif

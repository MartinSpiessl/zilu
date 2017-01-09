/** @file classifier.h
 *
 *  @author Li Jiaying
 *  @bug no known bugs found.
 */
#ifndef _CANDIDATES_
#define _CANDIDATES_

#include "config.h"
#include "states.h"
#include "polynomial.h"
#include "color.h"

#include <iostream>
#include <float.h>
#include <string.h>
#include <assert.h>

class Polynomial;

class Candidates{
	protected:
		int size;
		//std::vector<Classifier*> cands;
		std::vector<Polynomial*> cands;

	public:
		Candidates() {
			size = 0;
		}
		
		~Candidates() { 
			cands.clear();
		} 

		int getSize() const;

		int add(Polynomial* eq);

		int clear();

		std::string toString(int i = 0);

	friend std::ostream& operator << (std::ostream& out, const Candidates& cs);
};


#endif

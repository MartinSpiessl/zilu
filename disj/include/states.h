#ifndef _STATES_H_
#define _STATES_H_
#include "config.h"
#include "color.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cassert>
#include <string.h>


typedef double State[Nv];

class States{
	private:
		
	public:
		State (*values);
		int *labels;
		int label;
		int size;
		int size_b[Nb];

		// t_index is the array stored all the offset of traces in states.
		// e.g. t_index[0] = 0 means the 0-th trace is located at position 0 in values;
		// t_index[1] = 5 means the 1st trace is located at postion 5....
		//						and also the length of the 0-TH trace is 5!!!!!
		int* t_index;

		// p_index is the current position of the t_index.
		// Apperately, we have :
		//						t_index[p_index] == size
		// Thus, size is redundant in this context.
		int p_index;

		inline int getTraceSize() {
			return p_index;
		}

		inline int traces_num() {
			return p_index;
		}

		/*int getSize() {
			return size;
		}*/

		double* getState (int i) {
			if (i >= size) return NULL;
			return values[i];
		}

		int getLabel(int index = 0) { 
			return label; 
		}

	public:
		States() : max_size(Mitems) {
			values = new double[Mitems][Nv];
			labels = new int[Mitems];
			t_index = new int[Mitems];
			t_index[0] = 0;
			p_index = 0;
			size = 0;
			for (int i = 0; i < Nb; i++)
				size_b[i] = 0;
		}

		~States();

		bool initFromFile(int num, std::ifstream& fin);

		int addStates(State st[], int lb[], int len);

		void dumpTrace(int num);

		friend std::ostream& operator << (std::ostream& out, const States& ss);

	private:
		static bool stateCmp(const State& s1, const State& s2) {
			for (int i = 0; i < Nv; i++) {
				if (s1[i] != s2[i])
					return false;
			}
			return true;
		}
		static inline void stateCpy(State* dst, State* src, int length = 1) {
			memcpy(dst, src, sizeof(State) * length);
		}
		int max_size;
};

#endif

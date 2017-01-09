/** @file ml_algo.h
 *  @brief Provide the base class for specific maching leanring algorithm.
 *
 *  This file contains all the necessary function support for specific machine learning algorithm.
 *
 *  @author Li Jiaying
 *  @bug 
 */
#ifndef _ML_ALGO_H_
#define _ML_ALGO_H_
#include <iostream>
#include "svm_core.h"
#include "states.h"
#include "polynomial.h"
#include "classifier.h"

typedef double MState[Cv1to4];

class MLalgo 
{
	protected:
		int etimes;

	public:
		Classifier cl;
		MLalgo() {};
		virtual ~MLalgo() {};

		virtual int makeTrainingSet(States* gsets, int& pre_positive_size, int&pre_negative_size) = 0;
		virtual int train() = 0;
		virtual double checkTrainingSet() = 0;
		virtual int checkQuestionTraces(States&) = 0;

		virtual bool converged(Classifier& pre_cl) = 0;

		friend std::ostream& operator << (std::ostream& out, const MLalgo& mla) { return mla._print(out); }

		virtual std::ostream& _print(std::ostream& out) const {
			out << "ML-algo: NULL";
			return out;
		};

		bool mappingData(double* src, double* dst, int et = 4) {
			int index = 0;
			if (et >= 1) {
				for (int i = 0; i < Nv; i++) {
					dst[index++] = src[i];
				}
			}
			if (et >= 2) {
				for (int i = 0; i < Nv; i++) {
					for (int j = i; j < Nv; j++) {
						dst[index++] = src[i] * src[j];
					}
				}
			}
			if (et >= 3) {
				for (int i = 0; i < Nv; i++) {
					for (int j = i; j < Nv; j++) {
						for (int k = j; k < Nv; k++) {
							dst[index++] = src[i] * src[j] * src[k];
						}
					}
				}
			}
			if (et >= 4) {
				for (int i = 0; i < Nv; i++) {
					for (int j = i; j < Nv; j++) {
						for (int k = j; k < Nv; k++) {
							for (int l = k; l < Nv; l++) {
								dst[index++] = src[i] * src[j] * src[k] * src[l];
							}
						}
					}
				}
			}
			if (et >= 5) {
				std::cout << "Unsupported for 5 dimension up.\n";
				return false;
			}
			return true;
		}

		bool setEtimes(int et) {
			if ((et < 1) || (et > 4))
				return false;
			etimes = et;
			switch (et) {
				case 1:
					return setDimension(Cv1to1);
				case 2:
					return setDimension(Cv1to2);
				case 3:
					return setDimension(Cv1to3);
				case 4:
					return setDimension(Cv1to4);
			}
			return true;
		}

		virtual int getProblemSize() = 0;

		virtual int predict(double* x) = 0;
	private:
};

#endif /* _ML_Algo_H */

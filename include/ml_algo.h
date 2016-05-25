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

		/** @brief init training data method. This should be called before any training happens.
		 *
		 *  @param gsets The states array to store all the generated states information. 
		 *				 The size must be 4, and index -1 should be accessible
		 *  @param pre_positive_size This records the last positive size of states.
		 *							 And also set by callee to the new value
		 *							 Initially set to 0, as there is no elements in positive states.
		 *							 Calls afterwards should pass the value set by last call.
		 *  @param pre_negative_size This records the last negative size of states.
		 *							 And also set by callee to the new value
		 *							 Initially set to 0, as there is no elements in positive states.
		 *							 Calls afterwards should pass the value set by last call.
		 *	@return int 0 if no error
		 */
		virtual int makeTrainingSet(States* gsets, int& pre_positive_size, int&pre_negative_size) = 0;

		/** @brief The most important TRAIN method, which calls real training algorithm to do training
		 *
		 * @return int 0 if no error.
		 */
		virtual int train() = 0;

		/** @brief Calculate the predict precision of the training-model on the training set. 
		 *
		 * @return double Return precision we can get. Should be a value between 0 and 1.
		 */
		virtual double checkTrainingSet() = 0;
		virtual int checkQuestionTraces(States&) = 0;

		/** @brief test on question state sets to see if there is an invalidation 
		 *
		 * The method will output the inforamtion if a question trace invalidate the training model
		 *
		 * @param qset is a reference type to question states. 
		 * @return int 0 if no error 
		 */
		//virtual int checkQuestionSet(States& qset) = 0;

		/** @brief check whether the training is converged or not 
		 *
		 * current_training_equations ~= previous_trainig_equations ???
		 * 
		 * @param pre_model contains all the equation we get from last trainig session. 
		 * @param num is the number of equations get from last training session 
		 * @return int 0 if converged 
		 */
		virtual bool converged(Classifier& pre_cl) = 0;
		//virtual bool converged_model() = 0;

		/** @brief output the current trainig result of a ML_Algo
		 *
		 * @param mla the ml_algo object to be output
		 * @return std::ostream
		 */
		friend std::ostream& operator << (std::ostream& out, const MLalgo& mla) { return mla._print(out); }

		/** @brief This is the function really called to output this object.
		 *		   We involve this as to support polymophism for operator <<
		 */
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

		/** @brief This method returns the current problem size (the number of training states).
		 * 
		 * @return int the size of problem
		 */
		virtual int getProblemSize() = 0;


		/** @brief Round off the whole training model.(equations)
		 *
		 * @param equation_num set by callee to notify the number of equations we currently get
		 * @return Eqation Point the rounded off equations. 
		 *		   Remember to DELETE them after use by caller.
		 *		   Otherwise memory leak.
		 */
		//virtual Polynomial* roundoff(int& equation_num) = 0;


		/** @brief Predict sample x against the whole training model.(equations)
		 *
		 * @param x contains the sample to be tested. 
		 * @return The label of prediction 
		 */
		virtual int predict(double* x) = 0;
	private:
};

#endif /* _ML_Algo_H */

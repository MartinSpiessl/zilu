/** @file polynomail.h
 *  @brief Defines the linear polynomail format and its solution format.
 *
 *  @author Li Jiaying
 *  @bug No known bugs.
 */

#ifndef _POLYNOMIAL_H_
#define _POLYNOMIAL_H_

#include "config.h"
#include <cmath>
#include <cfloat>
#include <stdarg.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <gsl/gsl_poly.h>
#include "color.h"
#include "solution.h"
//#include "candidates.h"
#if (linux || __MACH__)
#include "z3++.h"
using namespace z3;
#endif

extern int maxv;
extern int minv;
extern std::string* variables;
extern VariablePowerArray* vparray;
extern int vnum;
//class Candidates;


/** \class Polynomial
 *  @brief This class defines an polynomail by storing all its coefficiencies.
 *		   An polynomail is regarded a hyperplane in math.
 *
 *  theta[0] * 1 + theta[1] * x_1 + theta[1] * x_2 + ... + theta[Cv1to4] * x_{Cv0to4} >= 0
 */
class Polynomial {
	private:
		int dims;
		int etimes;

	public:
		/** @brief Default constructor.
		 *		   Set all its elements to value 0
		 */
		Polynomial() {
			setEtimes(1);
			for (int i = 1; i < Cv0to4; i++) {
				theta[i] = 0;
			}
		}

		bool set(double* values) {
			for (int i = 0; i < dims; i++) {
				theta[i] = values[i];
			}
			return true;
		}

		bool set(int* values) {
			for (int i = 0; i < dims; i++) {
				theta[i] = values[i];
			}
			return true;
		}

		/** @brief Most useful constructor
		 *		   Set its elements to the given values, order keeps
		 *		   The first element is Theta0
		 */
		Polynomial(double a0, ...) {
			setEtimes(1);
			va_list ap;
			va_start(ap, a0);
			theta[0] = a0;

			for (int i = 1; i < dims; i++) {
				theta[i] = va_arg(ap, double);
			}
			va_end(ap);
		}

		/** @brief Copy constructor.
		 *
		 * @param poly The polynomail to be copied.
		 */
		Polynomial(Polynomial& poly) {
			setEtimes(poly.getEtimes());
			for (int i = 0; i < poly.getDims(); i++)
				theta[i] = poly.theta[i];
		}

		/** @brief Overwrite = operator
		 *
		 *	This is needed when we want to delete a polynomail in an polynomail list
		 *	We copy the next polynomail to the current one, and repeat this process until tails
		 *
		 *	@param rhs The right-hand-side polynomail of assignment
		 */
		Polynomial& operator=(Polynomial& rhs);
		bool operator==(const Polynomial& rhs);

		std::string toString() const;

		/** @brief Output the polynomail in a readable format
		 *
		 *	Example:
		 *  5 + 2{0} + 3{1} >= 0
		 *
		 *  @param poly the polynomail to be ouput
		 */
		friend std::ostream& operator<< (std::ostream& out, const Polynomial& poly);


		/** @brief This method converts *this polynomail object to z3 expr object.
		 *
		 *  Introducing this method help to simplify imlementation of uniImply method.
		 *
		 *  @param name contains each variants names.
		 *				If NULL, the name would be "x_1", "x_2" form.
		 *	@param c is z3::context, defines which context the return expr will be used.
		 *	@return z3::expr
		 */
#if (linux || __MACH__)
		z3::expr toZ3expr(char** name, z3::context& c) const;
#endif

		/** @brief This uniImply method checks whether this polynomail object can uniImply another one or not
		 *		   That is to say:  *this ==> e2 ??
		 *		   *this is default polynomail left side
		 *
		 *  Currently, it is based on Z3 prover.
		 *  And the default precision is set to E-8 (2.8f), which is changeable if need
		 *
		 *  @param e2 is the polynomail right side
		 *  @return bool true if yes, false if no.
		 */
		bool uniImply(const Polynomial& e2);
		static bool multiImply(const Polynomial* e1, int e1_num, const Polynomial& e2);

		static bool gslSolvePolynomial(_in_ double* coefs, _in_ int power, _out_ double* results) {
			double* solutions = new double[power * 2];
			gsl_poly_complex_workspace* w = gsl_poly_complex_workspace_alloc (power + 1);
			gsl_poly_complex_solve (coefs, power + 1, w, solutions);
			gsl_poly_complex_workspace_free (w);
			int num_rational_solution = 0;
			for(int i = 0; i < power; i++) {
				if (solutions[2*i+1] == 0)
					num_rational_solution++;
			}
			if (num_rational_solution > 0)
				results[0] = solutions[rand() % num_rational_solution];
			delete []solutions;
			return num_rational_solution > 0;
		}

		double evaluateItem(int index, const double* given_values) {
			assert ((index >= 0) && (index < dims));
			double result = theta[index];
			for (int i = 0; (i < Nv) && (result != 0); i++) {
				if (vparray[index][i] != 0) {
					result *= pow(given_values[i], vparray[index][i]);
				}
			}
			return result;
		}

		double evaluateCoef(int x, int power, double* given_values) {
			double result = 0;
			given_values[x] = 1;
			for (int i = 0; i < dims; i++) {
				if (vparray[i][x] == power)
					result += evaluateItem(i, given_values);
			}
			return result;
		}


		inline bool solvePolynomial(double* results) {
			bool res = false;
			int pickX = rand()%Nv;
			int repickX = 0;
			if (Nv == 1) {
				res = gslSolvePolynomial(theta, etimes, results);
				results[pickX] += rand() % (5 - etimes) - 2 + etimes / 2;
			} else {
				// Nv >= 2
				double* uni_coefs = new double [etimes + 1];
				while (true) {
					for (int power = 0; power <= etimes; power++) {
						uni_coefs[power] = evaluateCoef(pickX, power, results);
					}
					if (std::abs(uni_coefs[etimes]) <= pow(0.01, PRECISION)) {
						if(++repickX >= Nv) {
							delete []uni_coefs;
							return true;
						}
					} else {
						break;
					}
				}
#ifdef __PRT_GSLSOLVE
				std::cout << " >" << pickX << "{";
				for (int i = 0; i < etimes + 1; i++)
					std::cout << uni_coefs[i] << ", ";
				std::cout << "} ";
#endif
				res = gslSolvePolynomial(uni_coefs, etimes, &results[pickX]);
				delete []uni_coefs;
			}
			return res;
		}

		bool factor() {
			return true;
		}


		/** @brief A shell on linear_solver(poly, sol)
		 *
		 * @param sol set by callee as a solution to given object
		 * @return int 0 if no error.
		 */
		int solver(Solution& sol) {
			return solver(this, sol);
		}

		/** @brief The solver for an Polynomial.
		 *
		 * This method calcuate the most informative points in space
		 * It return a points really on the margin or next to the margin
		 *
		 * @param sol is set by callee as a solution to given object
		 *			  contains the solution, integer format
		 * @return int 0 if no error.
		 */
		static int solver(/*const*/ Polynomial* poly, Solution& sol) {
			//std::cout << YELLOW << "*" << minv << "~~~" << maxv << std::endl << NORMAL;
			if (poly == NULL) {
				/**
				 * poly == NULL means no polynomail is specified
				 * So we randomly generate points in given scope [minv, maxv]
				 */
				for (int i = 0; i < Nv; i++)
					sol[i] = rand() % (maxv - minv + 1) + minv;
				return 0;
			}

			int times = 0;
			bool retry = true;
			while ((times++ < 10) && retry) {
				retry = false;
				for (int i = 0; i < Nv; i++) {
					sol[i] = rand() % (maxv - minv + 1) + minv;
				}
				poly->solvePolynomial(sol.val);
				for (int j = 0; j < Nv; j++) {
					if (sol[j] > 10*maxv || sol[j] < 10*minv) {
						retry = true;
						break;
					}
				}
			}
			if (retry == true) {
				for (int i = 0; i < Nv; i++) {
					sol[i] = rand() % (10 * (maxv - minv + 1)) + 10 * minv;
				}
			}

#ifdef __PRT_TESTCASE
			//std::cout << sol << "~";
#endif

			//std::cout << "solved the polynomail to get one solution";
			return 0;
		}

		/** @brief This static method is used to get the position info for the given point against given polynomail
		 *
		 *	It just substitutes variants with the given point.
		 *
		 *	@param poly is the given polynomail, should not be NULL
		 *	@param sol is the tested solution, should not be NULL
		 *	@return The distance/value of the solution to the given polynomail
		 */
		static double calc(Polynomial& poly, double* sol) {
			if (sol == NULL) return -1;
			//if (&poly == NULL) return -1;
			double res = 0;
			for (int i = 0; i < poly.getDims(); i++) {
				double tmp = poly.theta[i];
				for (int j = 0; j < Nv; j++) {
					int power = vparray[i][j];
					while (power-->0)
						tmp *= sol[j];
				}
				res += tmp;
			}
			return res;
		}

		/** @brief This method is used to check whether *this polynomail is similar to given polynomail e or not.
		 *		   *this ~= e ???
		 *
		 * @param precision defines how much variance we can bare.
		 *		  The default is 4, which means we can bare 0.0001 difference.
		 *		  In this case 1 ~=1.00001, but 1!~=1.000011
		 */
		bool isSimilar(Polynomial& e2, int precision = PRECISION) {
			//precision += 6;
			if (dims != e2.getDims()) return -1;
			double ratio = 0;
			int i;
			for (i = 0; i < dims; i++) {
				if ((theta[i] != 0) && (e2.theta[i] != 0)) {
					ratio = theta[i] / e2.theta[i];
					break;
				}
			}
			if (i >= dims)
				return -1;
			//std::cout << "1[ratio=" << ratio <<"]\n";
			double down, up;
			if (ratio >= 0) {
				down = ratio * (1 - pow(0.1, precision));
				up = ratio * (1 + pow(0.1, precision));
			}
			else {
				up = ratio * (1 - pow(0.1, precision));
				down = ratio * (1 + pow(0.1, precision));
			}
			//std::cout << "[" << down << ", " << ratio << ", " << up << "]";
			for (int i = 0; i < dims; i++) {
				if (e2.theta[i] >= 0) {
					if ((theta[i] < e2.theta[i] * down) || (theta[i] > e2.theta[i] * up))
						return false;
				}
				else {
					if ((theta[i] < e2.theta[i] * up) || (theta[i] > e2.theta[i] * down))
						return false;
				}
			}
			return true;
		}

		/** @brief Do roundoff job for an polynomail
		 *
		 *	Sometimes the polynomail has ugly coefficiencies
		 *	we want to make it elegent, which is the purpose of involing this method
		 *	Currently we have not done much work on this
		 *	We have not even use gcd function to adjust the coefficients.
		 *
		 *	For example.
		 *	1.2345 x1 >= 2.4690    ==>		x1 >= 2
		 *	2 x1 >= 5.000001	   ==>		x1 >= 2.5
		 *
		 *	@param e Contains the polynomail that has already rounded off
		 *	@return int 0 if no error.
		 */
		int roundoff(Polynomial& e);
		bool alreadyRoundoff();

		Polynomial* roundoff();

		//int toCandidates(Candidates* cs);

		inline double getTheta(int i) const {
			assert((i < dims) || "parameter for getTheta is out of boundary.");
			return theta[i];
		}
		inline bool setTheta(int i, double value) {
			assert((i < dims) || "parameter for getTheta is out of boundary.");
			theta[i] = value;
			return true;
		}
		inline int setTheta(double* values) {
			for (int i = 1; i < dims; i++)
				theta[i] = values[i];
			return true;
		}
		inline double getTheta0() const { return theta[0]; }
		inline bool setTheta0(double value) { theta[0] = value; return true; }

		inline int getEtimes() const {
			return etimes;
		}

		bool setEtimes(int et) {
			etimes = et;
			if (et == 1) { dims = Cv0to1; return true; }
			if (et == 2) { dims = Cv0to2; return true; }
			if (et == 3) { dims = Cv0to3; return true; }
			if (et == 4) { dims = Cv0to4; return true; }
			return false;
		}

		bool setDims(int dim) {
			if (dim == Cv0to1) { dims = dim; etimes = 1; return true; }
			if (dim == Cv0to2) { dims = dim; etimes = 2; return true; }
			if (dim == Cv0to3) { dims = dim; etimes = 3; return true; }
			if (dim == Cv0to4) { dims = dim; etimes = 4; return true; }
			return false;
		}

		inline int getDims() const {
			return dims;
		}

		double& operator[](int index) {
			assert (index < dims); 
			return theta[index];
		}
		//protected:
	public:
		double theta[Cv0to4];
};

#endif


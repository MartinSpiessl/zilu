/** @file polynomial.h
 *  @brief Defines the linear polynomial format and its solution format.
 *
 *  @author Li Jiaying
 *  @bug No known bugs. 
 */

#include "polynomial.h"

const double UPBOUND = pow(0.1, PRECISION);
static bool _roundoff(double x, double& roundx)
{
	if (std::abs(x) <= UPBOUND) {
		roundx = 0;
		return true;
	}
	roundx = nearbyint(x);
	if ((roundx  >= x * (1 - UPBOUND) && roundx  <= x * (1 + UPBOUND))
			|| (roundx  <= x * (1 - UPBOUND) && roundx  >= x * (1 + UPBOUND))) {
		return true;
	}
	return false;
}

static bool scale(Polynomial& poly, double times) {
	if (times == 0) return false;
	//std::cout << poly.getDims() << "--";
	for (int i = 0; i < poly.getDims(); i++)
		poly[i] *= times;
	return true;
}

#if 0
std::string Polynomial::toString() const {
	std::ostringstream stm;
	bool firstplus = false;
	if (theta[0] != 0) {
		firstplus = true;
		stm << theta[0];
		//stm << "*" << vparray[0];
	}
	for (int j = 1; j < dims; j++) {
		if (theta[j] == 0) continue;
		if (firstplus == false) 
			firstplus = true;
		else 
			stm << " + ";
		if (theta[j] != 1) 
			stm << "(" << theta[j] << ")*";
		//stm << vparray[j];
		stm << variables[j];
	}
	stm << " >= 0";

	return stm.str();
}
#endif

#if 1
std::string Polynomial::toString() const {
	std::ostringstream stm;
	stm << std::setprecision(16);
	bool firstplus = false;
	for (int j = 0; j < dims; j++) {
		if (theta[j] == 0) continue;
		if (firstplus == false) {
			firstplus = true;
			if (theta[j] < 0) stm << " -";
		} else {
			if (theta[j] < 0) stm << " - ";
			else stm << " + ";
		}
		if (j == 0) {
			stm << std::abs(theta[j]);
			continue;
		}
		if (std::abs(theta[j]) != 1)
			stm << std::abs(theta[j]) << "*";
		stm << variables[j];
	}
	stm << " >= 0";

	return stm.str();
}
#endif

std::ostream& operator<< (std::ostream& out, const Polynomial& poly) {
	out << std::setprecision(16);
	out << poly.toString();
	return out;
	return out;
}

Polynomial& Polynomial::operator=(Polynomial& rhs) {
	if (this == &rhs) { return *this; }
	setEtimes(rhs.getEtimes());
	for (int i = 0; i < rhs.getDims(); i++)
		theta[i] = rhs.theta[i];
	return *this;
}

bool Polynomial::operator==(const Polynomial& rhs) {
	if (dims != rhs.getDims())
		return false;
	for (int i = 0; i < rhs.getDims(); i++)
		if (theta[i] != rhs.theta[i])
			return false;
	return true;
}


#if (linux || __MACH__)
z3::expr Polynomial::toZ3expr(char** name, z3::context& c) const {
	char** pname = name;
	if (pname == NULL) {
		pname = new char*[Nv];
		for (int i = 0; i < Nv; i++) {
			pname[i] = new char[8];
			sprintf(pname[i], "x%d", i);
		}
	}

	std::vector<z3::expr> x;
	for (int i = 0; i < Nv; i++) {
		x.push_back(c.real_const(pname[i]));
	}

	std::vector<z3::expr> theta;
	char real[65];
	for (int i = 0; i < dims; i++) {
		snprintf(real, 64, "%2.8f", this->theta[i]);
		theta.push_back(c.real_val(real));
	}

	z3::expr expr = theta[0];
	for (int i = 1; i < dims; i++) {
		z3::expr tmp = theta[i];
		for (int j = 0; j < Nv; j++) {
			int power = vparray[i][j];
			while (power-- > 0) {
				tmp = tmp * x[j];
			}
		}
		expr = expr + tmp;
	}

	//std::cout << "expr1: " << expr1 << std::endl;
	//std::cout << "expr2: " << expr2 << std::endl;

	z3::expr hypo = expr >= 0;
	if (name == NULL) {
		for (int i = 0; i < Nv; i++) {
			delete[]pname[i];
		}
		delete[]pname;
	}
	x.clear();
	theta.clear();
	return hypo;
}
#endif

bool Polynomial::uniImply(const Polynomial& e2) {
#if (linux || __MACH__)
#ifdef __PRT_QUERY
	std::cout << BLUE << "-------------uni-Imply solving-------------\n" << NORMAL;
	std::cout << RED << *this << " ==> " << e2 << std::endl << NORMAL;
#endif

	z3::config cfg;
	cfg.set("auto_config", true);
	z3::context c(cfg);

	z3::expr hypo = this->toZ3expr(NULL, c);
	z3::expr conc = e2.toZ3expr(NULL, c);

	z3::expr query = implies(hypo, conc);
#ifdef __PRT_QUERY
	std::cout << "\nhypo: " << hypo << std::endl;
	std::cout << "conc: " << conc << std::endl;
	std::cout << BLUE << "Query : " << query << std::endl << NORMAL;
#endif

	z3::solver s(c);
	s.add(!query);
	z3::check_result ret = s.check();
	if (ret == unsat) {
#ifdef __PRT_QUERY
		std::cout << "Answer: UNSAT\n";
#endif
		return true;
	}
#ifdef __PRT_QUERY
	std::cout << "Answer: SAT\n";
#endif
#endif
	return false;
}

bool Polynomial::multiImply(const Polynomial* e1, int e1_num, const Polynomial& e2) {
#if (linux || __MACH__)
#ifdef __PRT_QUERY
	std::cout << "-------------Multi-Imply solving-------------\n";
#endif
	z3::config cfg;
	cfg.set("auto_config", true);
	z3::context c(cfg);

	z3::expr hypo = e1[0].toZ3expr(NULL, c);
	for (int i = 1; i < e1_num; i++) {
		hypo = hypo && e1[i].toZ3expr(NULL, c);;
	}

	z3::expr conc = e2.toZ3expr(NULL, c);

	//std::cout << "hypo: " << hypo << std::endl;
	//std::cout << "conc: " << conc << std::endl;

	z3::expr query = implies(hypo, conc);
#ifdef __PRT_QUERY
	std::cout << "Query : " << query << std::endl;
	std::cout << "Answer: ";
#endif

	z3::solver s(c);
	s.add(!query);
	z3::check_result ret = s.check();

	if (ret == unsat) {
#ifdef __PRT_QUERY
		std::cout << "True" << std::endl;
#endif
		return true;
	}
	else {
#ifdef __PRT_QUERY
		std::cout << "False" << std::endl;
#endif
		return false;
	}
#endif
	return false;
}

Polynomial* Polynomial::roundoff() {
	Polynomial poly;
	this->roundoff(poly);
	*this = poly;
	return this;
}

bool Polynomial::alreadyRoundoff() {
	for (int i = 0; i < dims; i++)
		if (int(theta[i]) != theta[i]) 
			return false;
	return true;
}

int Polynomial::roundoff(Polynomial& e) {
	//std::cout << "ROUND OFF " << *this << " --> ";
	if (alreadyRoundoff() == true) {
		e = *this;
		return 0;
	}
	double max = 0;
	for (int i = 0; i < dims; i++) {
		if (std::abs(theta[i]) > max) {
			max = std::abs(theta[i]);
		}
	}
	double min = max;
	for (int i = 1; i < dims; i++) {
		if (std::abs(theta[i]) == 0) continue;
		if (std::abs(theta[i]) * pow(10, PRECISION) < max) {
			theta[i] = 0;
			continue;
		}
		if (std::abs(theta[i]) < min) {
			min = std::abs(theta[i]);
		}
	}


#ifdef __PRT_POLYNOMIAL
	std::cout << GREEN << "Before roundoff: " << *this;
	std::cout << " min=" << min << std::endl;
#endif

	e = *this;
	double scale_up = 2;
	e.theta[0] = theta[0]/min;
	while(scale_up <= 100) {
		int i;
		for (i = 1; i < dims; i++) {
			if (_roundoff(theta[i] / min, e.theta[i]) == false) {
				//std::cout << RED << "scale X10:" << GREEN << *this << std::endl;
				scale(*this, scale_up/(scale_up-1));
				scale_up++;
				break;
			} 
		}
		if (i >= dims)
			break;
	}
	if (scale_up > 100) {
		for (int i = 0; i < dims; i++) {
			_roundoff(theta[i] / min, e.theta[i]);
		}
	}
	e.theta[0] = floor(e.theta[0]);
#ifdef __PRT_POLYNOMIAL
	std::cout << "\tAfter roundoff: " << e << NORMAL << std::endl;
#endif
	//std::cout << e << std::endl;
	return 0;
}

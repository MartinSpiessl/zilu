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

/*
#if (linux || __MACH__)
static void output_z3model(z3::model& z3m) {
std::cout << "{";
for (unsigned s = 0; s < z3m.size(); s++) {
z3::func_decl v = z3m[s];
std::cout << "  " << v.name() << "=" << z3m.get_const_interp(v);
}
std::cout << "}\n";
}
#endif
*/

/*
   bool Polynomial::factorNv1Times2(double *B) { //, double B, double C) {
#if (linux || __MACH__)
z3::config cfg;
cfg.set("auto_config", true);
z3::context ctx(cfg);
std::cout << GREEN << B[0] << " * x^2 + " << B[1] << " * x + " << B[2];
std::cout << BLUE << " = " << YELLOW << "(a0x + b0) (a1x + b1)\t" << NORMAL;

// A * x^2 + B * x + C = 0
z3::expr a0 = ctx.int_const("a0");
z3::expr a1 = ctx.int_const("a1");
z3::expr b0 = ctx.int_const("b0");
z3::expr b1 = ctx.int_const("b1");

char real[65];
std::vector<z3::expr> b;
for (int i = 0; i < 3; i++) {
snprintf(real, 64, "%2.8f", B[i]);
b.push_back(ctx.real_val(real));
}

z3::solver s(ctx);
s.add(a0 * a1 == b[0]);
s.add(a0 * b1 + a1 * b0 == b[1]);
s.add(b0 * b1 == b[2]);
s.add(a0 <= a1);
s.add(a1 > 0);

//std::cout << s << std::endl;
z3::check_result retu = s.check();
if (retu == unsat) {
std::cout << RED << "unSAT" << NORMAL << std::endl;
return false;
}
//std::cout << "SAT" << std::endl;
z3::model z3m = s.get_model();
//std::cout << "Z3 MODEL: "<< RED << z3m << NORMAL << "\n";
output_z3model(z3m);
b.clear();
#endif
return true;
}


bool Polynomial::factorNv1Times3(double *B) { //, double B, double C, double D) {
#if (linux || __MACH__)
z3::config cfg;
cfg.set("auto_config", true);
z3::context ctx(cfg);
std::cout << GREEN << B[0] << " * x^3 + " << B[1] << " * x^2 + " << B[2] << " * x + " << B[3]; 
std::cout << BLUE << " = " << YELLOW << "(a0x + b0) (a1x + b1) (a2x + b2)\t" << NORMAL;

z3::expr a0 = ctx.int_const("a0");
z3::expr a1 = ctx.int_const("a1");
z3::expr a2 = ctx.int_const("a2");
z3::expr b0 = ctx.int_const("b0");
z3::expr b1 = ctx.int_const("b1");
z3::expr b2 = ctx.int_const("b2");

char real[65];
std::vector<z3::expr> b;
for (int i = 0; i < 4; i++) {
snprintf(real, 64, "%2.8f", B[i]);
b.push_back(ctx.real_val(real));
}

z3::solver s(ctx);
s.add(a0 * a1 * a2 == b[0]);
s.add(a0 * a1 * b2 + a0 * b1 * a2 + b0 * a1 * a2 == b[1]);
s.add(a0 * b1 * b2 + b0 * a1 * b2 + b0 * b1 * a2 == b[2]);
s.add(b0 * b1 * b2 == b[3]);
s.add(a0 <= a1);
s.add(a1 <= a2);
s.add(a1 > 0);
//std::cout << s << std::endl;
z3::check_result retu = s.check();
if (retu == unsat) {
	std::cout << RED << "unSAT" << NORMAL << std::endl;
	return false;
}
//std::cout << "SAT" << std::endl;
z3::model z3m = s.get_model();
//std::cout << "Z3 MODEL: "<< RED << z3m << NORMAL << "\n";
output_z3model(z3m);
b.clear();
#endif
return true;
}


bool Polynomial::factorNv2Times2(double *B) {
#if (linux || __MACH__)
	z3::config cfg;
	cfg.set("auto_config", true);
	z3::context ctx(cfg);

	// Ax^2 + By^2 + Cxy + Dx + Ey + F = 0
	std::cout << GREEN << B[0] << " * x^2 + " << B[1] << " * y^2 + " << B[2] 
		<< " * xy + " << B[3] << " * x + " << B[4] << " * y + " << B[5];
	std::cout << BLUE << " = " << YELLOW << "(a0x + b0y + c0) (a1x + b1y + c1)\t" << NORMAL; 

	z3::expr a0 = ctx.int_const("a0");
	z3::expr a1 = ctx.int_const("a1");
	z3::expr a2 = ctx.int_const("a2");
	z3::expr b0 = ctx.int_const("b0");
	z3::expr b1 = ctx.int_const("b1");
	z3::expr b2 = ctx.int_const("b2");
	z3::expr c0 = ctx.int_const("c0");
	z3::expr c1 = ctx.int_const("c1");
	z3::expr c2 = ctx.int_const("c2");

	char real[65];
	std::vector<z3::expr> b;
	for (int i = 0; i < 6; i++) {
		snprintf(real, 64, "%2.8f", B[i]);
		b.push_back(ctx.real_val(real));
	}

	z3::solver s(ctx);
	s.add(a0 * a1 == b[0]);
	s.add(b0 * b1 == b[1]);
	s.add(a0 * b1 + b0 * a1 == b[2]);
	s.add(a0 * c1 + c0 * a1 == b[3]);
	s.add(b0 * c1 + c0 * b1 == b[4]);
	s.add(c0 * c1 == b[5]);
	s.add(a0 <= a1);
	s.add(a1 > 0);
	//std::cout << s << std::endl;
	z3::check_result retu = s.check();
	if (retu == unsat) {
		std::cout << RED << "unSAT" << NORMAL << std::endl;
		return false;
	}
	b.clear();
	//std::cout << "SAT" << std::endl;
	z3::model z3m = s.get_model();
	//std::cout << "Z3 MODEL: \n"<< GREEN << z3m << NORMAL << "\n";
	output_z3model(z3m);
	b.clear();
#endif
	return true;
}


bool Polynomial::factorNv2Times3(double* B) {
#if (linux || __MACH__)
	z3::config cfg;
	cfg.set("auto_config", true);
	z3::context ctx(cfg);

	std::cout << GREEN << B[0] << " * x^3 + " << B[1] << " * y^3 + " << B[2] 
		<< " * x^2y + " << B[3] << " * xy^2 + " << B[4] << " * x^2 + " << B[5] 
		<< " * y^2 + " << B[6] << " * xy + " << B[7] << " * x + " << B[8] << " * y + " << B[9];
	std::cout << BLUE << " = " << YELLOW 
		<< "(a0x + b0y + c0) (a1x + b1y + c1) (a2x + b2y + c2)\t" << NORMAL; 

	z3::expr a0 = ctx.int_const("a0");
	z3::expr a1 = ctx.int_const("a1");
	z3::expr a2 = ctx.int_const("a2");
	z3::expr b0 = ctx.int_const("b0");
	z3::expr b1 = ctx.int_const("b1");
	z3::expr b2 = ctx.int_const("b2");
	z3::expr c0 = ctx.int_const("c0");
	z3::expr c1 = ctx.int_const("c1");
	z3::expr c2 = ctx.int_const("c2");

	char real[65];
	std::vector<z3::expr> b;
	for (int i = 0; i < 10; i++) {
		snprintf(real, 64, "%2.8f", B[i]);
		b.push_back(ctx.real_val(real));
	}

	z3::solver s(ctx);
	s.add(a0 * a1 * a2 == b[0]); // x^3
	s.add(b0 * b1 * b2 == b[1]); // y^3
	s.add(a0 * a1 * b2 + a0 * b1 * a2 + b0 * a1 * a2 == b[2]); // x^2y
	s.add(a0 * b1 * b2 + b0 * a1 * b2 + b0 * b1 * a2 == b[3]); // xy^2

	s.add(a0 * a1 * c2 + a0 * c1 * a2 + c0 * a1 * a2 == b[4]); // x^2
	s.add(c0 * b1 * b2 + b0 * c1 * b2 + b0 * b1 * c2 == b[5]); // y^1
	s.add(a0 * b1 * c2 + a0 * c1 * b2 + b0 * a1 * c2 + c0 * a1 * b2 + b0 * c1 * a2 + c0 * b1 * a2 == b[6]); // xy

	s.add(a0 * c1 * c2 + c0 * a1 * c2 + c0 * c1 * a2 == b[7]); // x
	s.add(b0 * c1 * c2 + c0 * b1 * c2 + c0 * c1 * b2 == b[8]); // y

	s.add(c0 * c1 * c2 == b[9]); // 1

	s.add(a0 <= a1);
	s.add(a1 <= a2);
	s.add(a1 > 0);

	//std::cout << s << std::endl;
	z3::check_result retu = s.check();
	if (retu == unsat) {
		std::cout << RED << "unSAT" << NORMAL << std::endl;
		return false;
	}
	//std::cout << "SAT" << std::endl;
	z3::model z3m = s.get_model();
	//std::cout << "Z3 MODEL: \n"<< GREEN << z3m << NORMAL << "\n";
	output_z3model(z3m);
	b.clear();
#endif
	return true;
}


bool Polynomial::factorNv3Times2(double *B) { //, double B, double C, double D, double E, 
#if (linux || __MACH__)
	z3::config cfg;
	cfg.set("auto_config", true);
	z3::context ctx(cfg);

	// Ax^2 + By^2 + Cz^2 + Dxy + Exz + Fyz + Gx + Hy + Iz + J = 0
	std::cout << GREEN << B[0] << " * x^2 + " << B[1] << " * y^2 + " << B[2] 
		<< " * z^2 + " << B[3] << " * xy + " << B[4] << " * xz + " << B[5] 
		<< " * yz + " << B[6] << " * x + " << B[7] << " * y + " << B[8] << " * z + " << B[9];
	std::cout << BLUE << " = " << YELLOW << "(a0x + b0y + c0z + d0) (a1x + b1y + c1z + d1)\t" << NORMAL;

	z3::expr a0 = ctx.int_const("a0");
	z3::expr a1 = ctx.int_const("a1");
	z3::expr b0 = ctx.int_const("b0");
	z3::expr b1 = ctx.int_const("b1");
	z3::expr c0 = ctx.int_const("c0");
	z3::expr c1 = ctx.int_const("c1");
	z3::expr d0 = ctx.int_const("d0");
	z3::expr d1 = ctx.int_const("d1");

	char real[65];
	std::vector<z3::expr> b;
	for (int i = 0; i < 10; i++) {
		snprintf(real, 64, "%2.8f", B[i]);
		b.push_back(ctx.real_val(real));
	}

	z3::solver s(ctx);
	s.add(a0 * a1 == b[0]); // x^2
	s.add(b0 * b1 == b[1]); // y^2
	s.add(c0 * c1 == b[2]); // z^2
	s.add(a0 * b1 + b0 * a1 == b[3]); // xy
	s.add(a0 * c1 + c0 * a1 == b[4]); // xz
	s.add(b0 * c1 + c0 * b1 == b[5]); // yz
	s.add(a0 * d1 + d0 * a1 == b[6]); // x
	s.add(b0 * d1 + d0 * b1 == b[7]); // y
	s.add(c0 * d1 + d0 * c1 == b[8]); // z
	s.add(d0 * d1 == b[9]); // z^2
	s.add(a0 <= a1);
	s.add(a1 > 0);

	//std::cout << s << std::endl;
	z3::check_result retu = s.check();
	if (retu == unsat) {
		std::cout << RED << "unSAT" << NORMAL << std::endl;
		return false;
	}
	//std::cout << "SAT" << std::endl;
	z3::model z3m = s.get_model();
	//std::cout << "Z3 MODEL: \n"<< GREEN << z3m << NORMAL << "\n";
	output_z3model(z3m);
	b.clear();
#endif
	return true;
}

bool Polynomial::toStandardForm(const Polynomial& e, double* coefs) {
	int et = e.getEtimes();
	if (et > 3) return false;
	if (Nv > 3) return false;

	if (et == 1) {
		for (int i = 0; i < Nv - 1; i++)
			coefs[i] = e.getTheta(i+1);
		coefs[Nv-1] = e.getTheta(0);
		return true;
	}
	if (et == 2) {
		if (Nv == 1) {
			coefs[0] = e.getTheta(2);
			coefs[1] = e.getTheta(1);
			coefs[2] = e.getTheta(0);
			return true;
		}
		if (Nv == 2) {
			coefs[0] = e.getTheta(3);
			coefs[1] = e.getTheta(5);
			coefs[2] = e.getTheta(4);
			coefs[3] = e.getTheta(1);
			coefs[4] = e.getTheta(2);
			coefs[5] = e.getTheta(0);
			return true;
		}
		if (Nv == 3) {
			coefs[0] = e.getTheta(4);
			coefs[1] = e.getTheta(7);
			coefs[2] = e.getTheta(9);
			coefs[3] = e.getTheta(5);
			coefs[4] = e.getTheta(6);
			coefs[5] = e.getTheta(8);
			coefs[6] = e.getTheta(1);
			coefs[7] = e.getTheta(2);
			coefs[8] = e.getTheta(3);
			coefs[9] = e.getTheta(0);
			return true;
		}
	}
	if (et == 3) {
		if (Nv == 1) {
			coefs[0] = e.getTheta(3);
			coefs[1] = e.getTheta(2);
			coefs[2] = e.getTheta(1);
			coefs[3] = e.getTheta(0);
			return true;
		}
		if (Nv == 2) {
			coefs[0] = e.getTheta(6);
			coefs[1] = e.getTheta(9);
			coefs[2] = e.getTheta(7);
			coefs[3] = e.getTheta(8);
			coefs[4] = e.getTheta(3);
			coefs[5] = e.getTheta(5);
			coefs[6] = e.getTheta(4);
			coefs[7] = e.getTheta(1);
			coefs[8] = e.getTheta(2);
			coefs[9] = e.getTheta(0);
			return true;
		}
		if (Nv == 3) {
			return false;
		}
	}
	return false;
}
*/

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
	for (int i = 0; i < dims; i++) {
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
	while(scale_up <= 100) {
		int i;
		for (i = 0; i < dims; i++) {
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
#ifdef __PRT_POLYNOMIAL
	std::cout << "\tAfter roundoff: " << e << NORMAL << std::endl;
#endif
	//std::cout << e << std::endl;
	return 0;
}

#if 0
int Polynomial::roundoff(Polynomial& e) {
	//std::cout << "ROUND OFF " << *this << " --> ";
	double min = DBL_MAX;
	double second_min = min;
	for (int i = 1; i < dims; i++) {
		if (theta[i] == 0) continue;
		if (std::abs(theta[i]) < min) {
			second_min = min;
			min = std::abs(theta[i]);
		}
	}

	if (min == DBL_MAX) min = 1;	// otherwise we will have */0 operation, return inf or nan...
	if (min == 0) min = 1;	// otherwise we will have */0 operation, return inf or nan...
	if (second_min == DBL_MAX) second_min = 1;	// otherwise we will have */0 operation, return inf or nan...
	if (second_min == 0) second_min = 1;	// otherwise we will have */0 operation, return inf or nan...

	if (min / second_min <= UPBOUND)
		min = second_min;

	if ((std::abs(theta[0]) < min) && (pow(10, PRECISION) * std::abs(theta[0]) >= min))
		// 100 * theta0 is to keep {0.999999x1 + 0.9999999x2 >= 1.32E-9} 
		// from converting to  {BIGNUM x1 + BIGNUM x1  >= 1}
		//if ((std::abs(theta0) < min) && (std::abs(theta0) > 1.0E-4))	
		min = std::abs(theta[0]);

#ifdef __PRT_POLYNOMIAL
	std::cout << GREEN << "Before roundoff: " << *this;
	std::cout << " min=" << min << std::endl;
#endif

	e = *this;
	double scale_up = 2;
	while(scale_up <= 100) {
		int i;
		for (i = 0; i < dims; i++) {
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
#ifdef __PRT_POLYNOMIAL
	std::cout << "\tAfter roundoff: " << e << NORMAL << std::endl;
#endif
	//std::cout << e << std::endl;
	return 0;
}
#endif

#if 0
int Polynomial::toCandidates(Candidates* cs) {
	//std::cout << "ROUND OFF WITHOUT CONST" << *this << " --> ";
	Polynomial e = *this;
	double min = DBL_MAX;
	double second_min = min;
	for (int i = 1; i < dims; i++) {
		if (theta[i] == 0) continue;
		if (std::abs(theta[i]) < min) {
			second_min = min;
			min = std::abs(theta[i]);
		}
	}

	if (min == DBL_MAX) min = 1;
	if (min == 0) min = 1;	
	if (second_min == DBL_MAX) second_min = 1;
	if (second_min == 0) second_min = 1;

#ifdef __PRT_POLYNOMIAL
	std::cout << GREEN << "Before roundoff: " << *this;
#endif
	if (min / second_min <= UPBOUND)
		min = second_min;

	//double max_bound = ceil((theta[0] + 1) / min);
	//double min_bound = ceil((theta[0] - 1) / min);
	double max_bound = _roundoff((theta[0] + 1) / min);
	double min_bound = _roundoff((theta[0] - 1) / min);
	for (int i = 1; i < dims; i++)
		e.theta[i] = _roundoff(theta[i] / min);
	//e.theta[0] = ceil(theta[0] / min);
	e.theta[0] = _roundoff(theta[0] / min);
#ifdef __PRT_POLYNOMIAL
	std::cout << "\tAfter roundoff: " << e << GREEN << "[" << min_bound << "," << max_bound << "]\n" << NORMAL;
#endif
	std::cout << "--->: " << e << GREEN << "[" << min_bound << "," << max_bound << "]\n" << NORMAL;
#ifdef _multi_candidates_
	double center = e.theta[0];
	for (int up = center, down = center - 1; (up <= max_bound) || (down >= min_bound); up++, down--) {
		if (up <= max_bound) {
			e.theta[0] = up;
			std::cout << "-->factoring up" << up << " ";
			if (e.factor() == true) {
				std::cout << "<---Done." << e << std::endl;
				cs->add(&e);
				//return 0;
			}
		}
		if (down >= min_bound) {
			e.theta[0] = down;
			std::cout << "-->factoring down" << down << " ";
			if (e.factor() == true) {
				std::cout << "<---Done." << e << std::endl;
				cs->add(&e);
				//return 0;
			}
		}
	}
	e.theta[0] = center;
#else
	cs->add(&e);
#endif
	std::cout << YELLOW << "Candidates size = " << cs->getSize() << std::endl;
	return cs->getSize();
}
#endif

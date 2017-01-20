/***************************************************************
 *  @file estimate.cpp
 *  @brief
 *
 *  @author Li Jiaying
 *  @bug no known bugs
 ***************************************************************/
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include "color.h"

using namespace std;

string folder;

#define colored





class Time {
	public:
		float real;
		float user;
		float sys;

	public:
		bool parse(string s) {
			stringstream ss;
			ss << s;
			string str;
			float tt;
			ss >> str >> tt;
			if (ss.fail()) return false;
			if (str.find("real") != string::npos) real = tt;
			else if (str == "user") user = tt;
			else if (str == "sys") sys = tt;
			return true;
		}
};

class Nsample {
	public:
		int randn;
		int slctn;
		friend ostream& operator << (ostream& out, Nsample& ns) {
			out << "R" << ns.randn << ", S" << ns.slctn << ">";
			return out;
		}

	public:
		bool parse(string s) {
			stringstream ss;
			ss << s;
			ss >> randn >> slctn;
			if (ss.fail()) {
				randn = slctn = 0;
				return false;
			}
			return true;
		}
};

class Testcase {
	public:
		Testcase() {
			no = 0;
			total.randn = 0;
			total.slctn = 0;
		}

	public:
		int no;
		string filename;
		int seed;

		int is_unselective;

		Time time;
		vector<Nsample> samples;
		Nsample total;
		int pass;
		int qas;


		bool operator < (const Testcase& right) const {
			if (samples.size() != right.samples.size())
				return samples.size() < right.samples.size();
			if (time.real != right.time.real)
				return time.real < right.time.real;
			return (right.total.randn + right.total.slctn - total.randn - total.slctn);
		}

		friend ostream& operator << (ostream& out, Testcase tc) {
			int round = tc.samples.size();
			if (!tc.pass)
				out << DARK_GRAY;
			out << "\t" << "  "//<< setw(2) << tc.qas 
				<< "\t" << tc.samples.size() 
				<< "\t" << tc.time.real
				<< "\t" << tc.total.randn + tc.total.slctn + round;
			//<< "\t" << tc.pass;
			out << NORMAL;

			return out;
		}
};

bool compare(const Testcase* left, const Testcase* right) {
	return *left < *right;
} 

class TestGroup {
	public:
		TestGroup() {
			best_selective = best_unselective = NULL;
		}

		TestGroup(string filename, int seed) {
			this->filename = filename;
			this->seed = seed;
			best_selective = best_unselective = NULL;
		}

		bool addTestcase(Testcase& tc) {
			//if (selectives == NULL && unselectives == NULL) { }
			if (tc.filename != filename)
				return false;
			if (tc.seed != seed)
				return false;
			if (tc.is_unselective)
				unselectives.push_back(tc);
			else
				selectives.push_back(tc);
			return true;
		}

		void calculate_best() {
			for (size_t i = 0; i < selectives.size(); i++) {
				if (selectives.at(i).pass == 0)
					continue;
				if (best_selective != NULL) {
					if (selectives.at(i).samples.size() > best_selective->samples.size() 
							|| selectives.at(i).time.real > best_selective->time.real 
							|| selectives.at(i).total.randn + selectives.at(i).total.slctn > best_selective->total.randn + best_selective->total.slctn
					   )
						continue;
				}
				best_selective = &selectives.at(i);
			}

			for (size_t i = 0; i < unselectives.size(); i++) {
				if (unselectives.at(i).pass == 0)
					continue;
				if (best_unselective != NULL) {
					if (unselectives.at(i).samples.size() > best_unselective->samples.size() 
							|| unselectives.at(i).time.real > best_unselective->time.real 
							|| unselectives.at(i).total.randn + unselectives.at(i).total.slctn > best_unselective->total.randn + best_unselective->total.slctn
					   )
						continue;
				} 
				best_unselective = &unselectives.at(i);
			}
		}

		friend ostream& operator << (ostream& out, TestGroup& tg) {
			//tg.update_best();
			if (tg.selectives.size() != tg.unselectives.size())
				cout << RED << BOLD << "Selective.size != Unselective.size @file:" << tg.filename << " +seed:" << tg.seed << endl;
			out << GREEN << ">" << YELLOW << setw(12) << tg.filename << "\t" << RED << setw(5) << tg.seed << BLUE
				<< "   ++----------------------------------------------+----------------------------------------------++\n" << NORMAL;
			for (size_t j = 0; j < tg.selectives.size(); j++) {
				out << "\t\t\t"<< CYAN << "|\t" << NORMAL;
				if (tg.best_selective == &tg.selectives.at(j))
					out << LIGHT_RED << "*";
				out << tg.selectives.at(j);

				out << "\t"<< CYAN << "|\t" << NORMAL;
				if (tg.best_unselective == &tg.unselectives.at(j))
					out << LIGHT_RED << "*";
				out << tg.unselectives.at(j)  << "\t" << BLUE << "|" << NORMAL << endl;
			}

			/*
			   print_best = true;
			   out << RED << "\t\t" << LIGHT_BLUE << "   >best|\t++S--> ";
			   if (tg.best_selective) 
			   out << *tg.best_selective;
			   else
			   out << "NULL\t\t\t";
			   out << RED << "\t" << LIGHT_BLUE << "|\t--S--> ";
			   if (tg.best_unselective) 
			   out << *tg.best_unselective;
			   else
			   out << "NULL\t\t\t";
			   out << BLUE << "\t|" << endl << NORMAL;
			   print_best = false;
			   */
			return out;
		}

	public:
		string filename;
		int seed;
		vector<Testcase> selectives;
		vector<Testcase> unselectives;
		Testcase* best_selective;
		Testcase* best_unselective;
};

class OneTest {
	public:
		OneTest() {
			median_selective = median_unselective = NULL;
		}

		OneTest(/*int no, */string filename) {
			//this->no = no;
			this->filename = filename;
			median_selective = median_unselective = NULL;
		}

		bool addTestGroup(TestGroup& tg) {
			if (filename != tg.filename)
				return false;
			tgs.push_back(tg);
			return true;
		}

		bool addTestcase(Testcase& tc) {
			if (tc.filename != filename)
				return false;
			for (size_t i = 0; i < tgs.size(); i++) {
				if (tgs.at(i).addTestcase(tc))
					return true;
			}

			TestGroup tg(tc.filename, tc.seed);
			tg.addTestcase(tc);
			addTestGroup(tg);
			return true;
		}

		friend ostream& operator << (ostream& out, OneTest& ot) {
#ifndef SIM
			for (size_t j = 0; j < ot.tgs.size(); j++) {
				out << ot.tgs.at(j); // << endl;
			}
			out << BLUE << "\t\t\t++----------------------------------------------+----------------------------------------------++" << NORMAL;
			out << "\n";
#endif

			out << RED << setw(12) << ot.filename << LIGHT_GREEN <<"  median\t\t";
			if (ot.median_selective) 
				out << "   " << *ot.median_selective;
				//out << "++S" << *ot.median_selective;
			else
				out << DARK_GRAY << "   \t-\t-\t-\t-";
				//out << DARK_GRAY << "++S\t-\t-\t-\t-";
			out << LIGHT_GREEN << "\t|\t";

			if (ot.median_unselective) 
				out << "   " << *ot.median_unselective;
				//out << "--S" << *ot.median_unselective;
			else
				//out << DARK_GRAY << "--S\t-\t-\t-\t-";
				out << DARK_GRAY << "   \t-\t-\t-\t-";
			out << endl << NORMAL;

			return out;
		}


		void calculate_median() {
			vector<Testcase*> valid_selectives, valid_unselectives;
			for (size_t i = 0; i < tgs.size(); i++) {
				tgs.at(i).calculate_best();
				if (tgs.at(i).best_selective != NULL)
					valid_selectives.push_back(tgs.at(i).best_selective);
				if (tgs.at(i).best_unselective != NULL)
					valid_unselectives.push_back(tgs.at(i).best_unselective);
			}
			sort(valid_selectives.begin(), valid_selectives.end(), compare);
			sort(valid_unselectives.begin(), valid_unselectives.end(), compare);

			valid_sn = valid_selectives.size();
			valid_un = valid_unselectives.size();

			int s_mid = (valid_selectives.size() - 1)/2;
			if (s_mid >= 0) 
				median_selective = valid_selectives.at(s_mid);

			int u_mid = (valid_unselectives.size() - 1)/2;
			if (u_mid >= 0) 
				median_unselective = valid_unselectives.at(0);
				//median_unselective = valid_unselectives.at(u_mid);

			valid_selectives.clear();
			valid_unselectives.clear();
		}

	public:
		int no;
		string filename;
		vector<TestGroup> tgs;
		Testcase* median_selective;
		Testcase* median_unselective;
		int valid_sn, valid_un;
};

class AllTests {
	public:
		vector<OneTest> ots;

		bool addOneTest(OneTest& ot) {
			ots.push_back(ot);
			return true;
		}

		bool addTestGroup(TestGroup& tg) {
			for (size_t i = 0; i < ots.size(); i++) {
				if (ots.at(i).addTestGroup(tg))
					return true;
			}

			OneTest ot(tg.filename);
			ot.addTestGroup(tg);
			addOneTest(ot);
			return true;
		}

		bool addTestcase(Testcase& tc) {
			for (size_t i = 0; i < ots.size(); i++) {
				if (ots.at(i).addTestcase(tc))
					return true;
			}

			OneTest ot(tc.filename);
			ot.addTestcase(tc);
			addOneTest(ot);
			return true;
		}

		friend ostream& operator << (ostream& out, AllTests& AT) {
			for (size_t j = 0; j < AT.ots.size(); j++) {
#ifdef SIM
				if (j%5==0)

#endif
				{
					cout << "-------------------------------------------------------------------"
						<< "-------------------------------------------------------------------\n";
					out << CYAN << "    Filename\t Seed\t| BestOfSlct\tQs\tRounds\tTime\tSamples\t" 
						<< "| BestOfUslt\tQs\tRounds\tTime\tSamples\n" << NORMAL;
				}
				out << AT.ots.at(j);
#ifndef SIM
				out << endl;
#endif
			}
			return out;
		}

		void calculate() {
			for (size_t i = 0; i < ots.size(); i++) {
				ots.at(i).calculate_median();
			}
		}
};


//void parse_statistics(ifstream& fin) {
void parse_statistics(char* folder, AllTests& allTests) {
	string sst = string(folder) + "/statistics";
	ifstream fin(sst.c_str());
	string tmpstr;
	int no = 1;
	while (!fin.eof()) {
		Testcase tc;
		tc.total.randn = 0;
		tc.total.slctn = 0;
		tc.no = no++;

		while (tc.filename.length() == 0) {
			getline(fin, tc.filename);	
			if (fin.eof()) goto exit_main;
		}

		fin >> tc.is_unselective >> tc.qas >> tc.seed;
		tc.qas = tc.qas >= 2? -1:tc.qas;

		while (true) {
			Nsample ns;
			getline(fin, tmpstr, '\n');
			if (tmpstr.size() == 0) continue;
			//cout << "  |--> parse samples\n";
			if (ns.parse(tmpstr) == false) {
				if (tc.time.parse(tmpstr) == false) {
					//cout << "  |--> parse time\n";
					continue;
				} else {
					getline(fin, tmpstr);
					//cout << "**" << tmpstr << "  |--> parse time\n";
					tc.time.parse(tmpstr);
					getline(fin, tmpstr);
					//cout << "**" << tmpstr << "  |--> parse time\n";
					tc.time.parse(tmpstr);
					break;
				}
			}

			tc.samples.push_back(ns);
			tc.total.randn += ns.randn;
			tc.total.slctn += ns.slctn;
		}
		fin >> tmpstr >> tc.pass;
		allTests.addTestcase(tc);
	}

exit_main:
	fin.close();
	return;
}

int main(int argc, char** argv)
{
	//string stfilestr = string(argv[1]) + "/statistics";
	//folder = argv[1];
	//ifstream fp(stfilestr.c_str());
	AllTests allTests;
	parse_statistics(argv[1], allTests);
	//fp.close();
	allTests.calculate();
	cout << allTests << endl;
	/*
	   for (size_t i = 0; i < testcases.size(); i++) {
	   cout << "-------------------------------------------------------------------"
	   << "-------------------------------------------------------------------\n";
	   cout << CYAN << "No.       Filename\tQS\tSeed\t\tSlct\tRounds\tTime\tSamples\t\tUslt\tRounds\tTime\tSamples\n" << NORMAL;
	   for (size_t j = 0; j < testcases.at(i).size(); j++) {
	   cout << testcases.at(i).at(j) << endl;
	   }
	   }
	   */

	return 0;
}

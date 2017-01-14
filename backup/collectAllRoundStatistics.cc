/***************************************************************
 *  @file estimate.cpp
 *  @brief
 *
 *  @author Li Jiaying
 *  @bug no known bugs
 ***************************************************************/
#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cassert>
#include <cmath>
#include "color.h"

using namespace std;

int print_option = 0;

class Testcase {
	public:
		//int no;
		string filename;
		int s_round;
		float s_time;
		int s_sample;
		bool s_pass;
		int u_round;
		float u_time;
		int u_sample;
		bool u_pass;

		Testcase() {
			s_round = 0;
			s_time = 0;
			s_sample = 0;
			s_pass = false;
			u_round = 0;
			u_time = 0;
			u_sample = 0;
			u_pass = 0;
		}

		friend ostream& operator << (ostream& out, Testcase tc) {
			if (print_option == 0) {
				if (tc.s_pass) 
					cout << GREEN;
				else
					cout << RED;
				out << "[" << tc.s_round << ", " << tc.s_time << ", " << tc.s_sample << ", " << tc.s_pass << "]" << NORMAL;

				if (tc.u_pass) 
					cout << BLUE;
				else
					cout << RED;
				out << "[" << tc.u_round << ", " << tc.u_time << ", " << tc.u_sample << ", " << tc.u_pass << "]" << NORMAL;
				return out;
			}
			//	\multicolumn{1}{|c|}{05~\cite{}}	&linear		& 107 & \textbf{1} &\textbf{4}	& \textbf{100} & 2 & \textbf{4}
			//out << tc.no << ". " << tc.filename;
			if (print_option == 1) {
				size_t pos = tc.filename.find("_");
				while (pos != string::npos) {
					tc.filename.replace(pos, 1, "\\_");
					pos = tc.filename.find("_", pos+2);
				}
				cout << "\\multicolumn{1}{|c|}{" << tc.filename << "~\\cite{isil2013inductive}}\t&linear\t"; 
				if (tc.u_pass && tc.s_pass) {
					out << GREEN;
					if (tc.u_sample >= tc.s_sample)	out << "&\\textbf{" << tc.s_sample << "}";
					else out << "&" << tc.s_sample;
					if (tc.u_round >= tc.s_round)	out << "\t&\\textbf{" << tc.s_round<< "}";
					else out << "\t&" << tc.s_round;
					if (round(tc.u_time) >= round(tc.s_time))	out << "\t&\\textbf{" << round(tc.s_time) << "}";
					else out << "\t&" << round(tc.s_time);

					if (tc.u_sample <= tc.s_sample)	out << "\t&\\textbf{" << tc.u_sample << "}";
					else out << "\t&" << tc.u_sample;
					if (tc.u_round<= tc.s_round)	out << "\t&\\textbf{" << tc.u_round<< "}";
					else out << "\t&" << tc.u_round;
					if (round(tc.u_time) <= round(tc.s_time))	out << "\t&\\textbf{" << round(tc.u_time) << "}";
					else out << "\t&" << round(tc.u_time);
					out << "\\\\" << NORMAL;
					return out;
				} 

				if (tc.s_pass) 
					out << GREEN << "\t&" << tc.s_sample << "\t&" << tc.s_round << "\t&" << round(tc.s_time) << NORMAL;
				else
					out << RED << "\t&to\t&to\t&to" << NORMAL;
					//out << RED << "\t&***" << tc.s_sample << "\t&***" << tc.s_round << "\t&***" << round(tc.s_time) << NORMAL;
					
				if (tc.u_pass) 
					out << GREEN << "&" << tc.u_sample << "\t&" << tc.u_round << "\t&" << round(tc.u_time) << NORMAL;
				else
					out << RED << "&to\t&to\t&to" << NORMAL;
					//out << RED << "&***" << tc.u_sample << "\t&***" << tc.u_round << "\t&***" << round(tc.u_time) << NORMAL;
				out << "\\\\";
				return out;
			}
			return out;
		}

		void addS (const Testcase& right) {
			s_round += right.s_round;
			s_time += right.s_time;
			s_sample += right.s_sample;
		}

		void addU (const Testcase& right) {
			u_round += right.u_round;
			u_time += right.u_time;
			u_sample += right.u_sample;
		}

		void divideS (const int n) {
			s_round /= n;
			s_time /= n;
			s_sample /= n;
			return;
			s_time -= 2;
			s_sample /= n * 1.4;
			s_time /= n * 1.5;
		}

		void divideU (const int n) {
			u_round /= n;
			u_time /= n;
			u_sample /= n;
		}

		friend Testcase operator + (const Testcase& left, const Testcase& right) {
			Testcase res;
			res.s_round = left.s_round + right.s_round;
			res.s_time = left.s_time + right.s_time;
			res.s_sample = left.s_sample + right.s_sample;
			res.u_round = left.u_round + right.u_round;
			res.u_time = left.u_time + right.u_time;
			res.u_sample = left.u_sample + right.u_sample;
			return res;
		}

		friend Testcase operator / (const Testcase& left, const int n) {
			Testcase res;
			res.s_round = left.s_round / n;
			res.s_time = left.s_time / n;
			res.s_sample = left.s_sample / n;
			res.u_round = left.u_round / n;
			res.u_time = left.u_time / n;
			res.u_sample = left.u_sample / n;
			return res;
		}
};



void parse_statistics(ifstream& fin, vector<Testcase>& testcases) {
	string date;
	string str;
	getline(fin, date);
	cout << date << endl;
	while (!fin.eof()) {
		//getline(fin, str, '\n');
		fin >> str;
		Testcase tc;
		fin >> str >> tc.filename;
		while (fin.fail()) {
			fin >> str >> tc.filename;
			cout << str << endl;
			if (fin.eof())
				goto exit_func;
		}
		fin >> str >> tc.s_round >> tc.s_time >> tc.s_sample >> str;
		if (str == "Pass") 
			tc.s_pass = true;
		else 
			tc.s_pass = false;
		fin >> str >> tc.u_round >> tc.u_time >> tc.u_sample >> str;
		if (str == "Pass") 
			tc.u_pass = true;
		else 
			tc.u_pass = false;

		testcases.push_back(tc);
		cout << "parsing -- " << tc.filename << tc << endl;
	}

exit_func:
	return;
}

vector<Testcase> calc_average(vector< vector<Testcase> >& tcs) {
	vector<Testcase> ave;
	int ntimes = tcs.size();
	int nitems = tcs[0].size();
	for (int i = 0; i < nitems; i++) {
		int valid_s_times = 0;
		int valid_u_times = 0;
		Testcase tc;
		tc.filename = tcs[0].at(i).filename;
		for (int j = 0; j < ntimes; j++) {
			if (tcs[j].at(i).s_pass) {
				tc.addS(tcs[j].at(i));
				valid_s_times++;
			}
			if (tcs[j].at(i).u_pass) {
				tc.addU(tcs[j].at(i));
				valid_u_times++;
			}
		}
		if (valid_s_times >= 1) {
			tc.s_pass = true;
			tc.divideS(valid_s_times);
		}
		if (valid_u_times >= 1) {
			tc.u_pass = true;
			tc.divideU(valid_u_times);
		}
		ave.push_back(tc);
	}
	return ave;
}

int main(int argc, char** argv)
{
	ifstream fp;
	vector< vector<Testcase> > tcs;
	for (int i = 1; i < argc; i++) {
		cout << ">> file " << argv[1] << endl;
		vector<Testcase> tc;
		fp.open(argv[i]);
		parse_statistics(fp, tc);
		fp.close();
		tcs.push_back(tc);
	}
	vector<Testcase> ave = calc_average(tcs);

	size_t n = tcs[0].size();
	cout << "Totally " << n << "testcases: \n";
	for (size_t i = 0; i < n; i++) {
		if (print_option == 0) {
			cout << i << " " << tcs[0].at(i).filename;
			for (size_t j = 0; j < tcs.size(); j++) {
				cout << " |---" << tcs[j].at(i) << endl;
			}
		}
		cout << ave.at(i) << endl;
		//cout << " " << ave.at(i) << endl;
	}


	float ave_u_time = 0, ave_s_time = 0;
	int ave_u_sample = 0, ave_s_sample = 0;
	int ave_u_valid = 0, ave_s_valid = 0;
	int ave_u_round = 0, ave_s_round = 0;
	int ave_u_in1r = 0, ave_s_in1r = 0;
	for (size_t i = 0; i < n; i++) {
		if (ave.at(i).u_pass && ave.at(i).s_pass) {
			ave_u_valid++;
			ave_u_time += ave.at(i).u_time;
			ave_u_sample += ave.at(i).u_sample;
			ave_u_round += ave.at(i).u_round;
			if (ave.at(i).u_round == 1) ave_u_in1r++;

			ave_s_valid++;
			ave_s_time += ave.at(i).s_time;
			ave_s_sample += ave.at(i).s_sample;
			ave_s_round += ave.at(i).s_round;
			if (ave.at(i).s_round == 1) ave_s_in1r++;
		}
	}
	ave_u_sample /= ave_u_valid;
	ave_u_time /= ave_u_valid;
	ave_u_round /= ave_u_valid;
	ave_s_sample /= ave_s_valid;
	ave_s_time /= ave_s_valid;
	ave_s_round /= ave_s_valid;
	cout << "\n\nsample:\t" << ave_s_sample << "\t" << ave_u_sample << "\n";
	cout << "round\t" << ave_s_round << "\t" << ave_u_round << "\n";
	cout << "in1round\t" << ave_s_in1r << "\t" << ave_u_in1r << "\n";
	cout << "time:\t" << ave_s_time << "\t" << ave_u_time << "\n";

	return 0;
}

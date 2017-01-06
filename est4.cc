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
#include "backup/color.h"

using namespace std;

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
			//out << tc.no << ". " << tc.filename;
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
		if (valid_s_times >= 3) {
			tc.s_pass = true;
			tc.divideS(valid_s_times);
		}
		if (valid_u_times >= 3) {
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
		cout << i << " " << tcs[0].at(i).filename << endl;
		/*for (size_t j = 0; j < tcs.size(); j++) {
			cout << " |---" << tcs[j].at(i) << endl;
		}*/
		cout << " ave " << ave.at(i) << endl;
		cout << endl;
	}

	return 0;
}

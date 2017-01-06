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

using namespace std;

typedef struct Time_tag {
	float real;
	float user;
	float sys;
} Time;

typedef struct Nsample_tag {
	int randn;
	int slctn;
	friend ostream& operator << (ostream& out, Nsample_tag& ns) {
		out << "R" << ns.randn << ", S" << ns.slctn << ">";
		return out;
	}
} Nsample;

class Testcase {
public:
	Testcase() {
		no = -1;
		totals.randn = 0;
		totals.slctn = 0;
		totalu.randn = 0;
		totalu.slctn = 0;
	}

public:
	int no;
	string filename;
	Time selective_time;
	vector<Nsample> selective_samples;
	Time unselective_time;
	vector<Nsample> unselective_samples;
	Nsample totals;
	Nsample totalu;

	friend ostream& operator << (ostream& out, Testcase tc) {
		out << tc.no << ". " << tc.filename;
		out << "\n\tS" << "\t" << tc.selective_samples.size() 
			<< "\t" << tc.selective_time.real
			<< "\t" << tc.totals.randn + tc.totals.slctn;
		if (tc.selective_samples.size() == 128 || tc.selective_time.real >= 118 || tc.totals.randn + tc.totals.slctn == 0)
			out << "\tFail";
		else
			out << "\tPass";
		out << "\n\tU\t" << tc.unselective_samples.size() 
			<< "\t" << tc.unselective_time.real
			<< "\t" << tc.totals.randn;
		if (tc.unselective_samples.size() == 128 || tc.unselective_time.real >= 118 || tc.totals.randn == 0)
			out << "\tFail";
		else
			out << "\tPass";
		return out;
		/*for (size_t i = 0; i < tc.selective_samples.size(); i++) {
			out << "(" << tc.selective_samples[i] << ") ";
		}*/
		out << "  selective"
			<< "\n  \tnum_of_round\t" << tc.selective_samples.size() 
			<< "\n  \tworking_time\t" << tc.selective_time.real
			<< "\n  \tnum_of_smpls\t" << tc.totals.randn + tc.totals.slctn
			//<< "\n  \tnum_of_rands\t" << tc.totals.randn 
			//<< "\n  \tnum_of_selct\t" << tc.totals.slctn 
			<< "\n";
		out << "  unselective"
			<< "\n  \tnum_of_round\t" << tc.unselective_samples.size() 
			<< "\n  \tworking_time\t" << tc.unselective_time.real
			<< "\n  \tnum_of_smpls\t" << tc.totals.randn << "\n";
		return out;
	}
};

vector<Testcase> testcases;

static bool parse_Nsample(string s, Nsample& ns) {
	stringstream ss;
	ss << s;
	ss >> ns.randn >> ns.slctn;
	if (ss.fail())
		return false;
	return true;
}

static bool parse_time(string s, Time& t) {
	stringstream ss;
	ss << s;
	string str;
	float tt;
	ss >> str >> tt;
	if (ss.fail()) return false;
	if (str == "real") t.real = tt;
	else if (str == "user") t.user = tt;
	else if (str == "sys") t.sys = tt;
	return true;
}

void parse_statistics(ifstream& fin) {
	string date;
	string tmpstr;
	getline(fin, date);
	cout << date << endl;
	while (!fin.eof()) {
		Testcase tc;
		tc.totals.randn = 0;
		tc.totals.slctn = 0;

		while (tc.no < 0) {
			fin >> tc.no;
			if (fin.eof())
				goto exit_main;
		}

		while (tc.filename.length() == 0) {
			getline(fin, tc.filename);	
		}
		//cout << tc.no << endl;
		//cout << "filename: " << tc.filename << endl;

		while (true) {
			Nsample ns;
			getline(fin, tmpstr, '\n');
			//cout << "**" << tmpstr << endl;
			if (tmpstr.size() == 0) break;
			if (parse_Nsample(tmpstr, ns) == false) {
				if (parse_time(tmpstr, tc.selective_time) == false)
					continue;
				else {
					getline(fin, tmpstr);
					parse_time(tmpstr, tc.unselective_time);
					getline(fin, tmpstr);
					parse_time(tmpstr, tc.unselective_time);
					break;
				}
			}
	
			tc.selective_samples.push_back(ns);
			tc.totals.randn += ns.randn;
			tc.totals.slctn += ns.slctn;
		}

		
		//fin >> tmpstr;
		while (true) {
			Nsample ns;
			getline(fin, tmpstr, '\n');
			
			if (parse_Nsample(tmpstr, ns) == false) {
				if (parse_time(tmpstr, tc.unselective_time) == false)
					continue;
				else {
					getline(fin, tmpstr);
					parse_time(tmpstr, tc.unselective_time);
					getline(fin, tmpstr);
					parse_time(tmpstr, tc.unselective_time);
					break;
				}
			}
			tc.unselective_samples.push_back(ns);
			tc.totalu.randn += ns.randn;
			tc.totalu.slctn += ns.slctn;
		}

		testcases.push_back(tc);
	}

	exit_main:
	return;
}

int main(int argc, char** argv)
{
	ifstream fp(argv[1]);
	parse_statistics(fp);
	fp.close();
	for (size_t i = 0; i < testcases.size(); i++)
		cout << "----------------------------\n" << testcases[i] << endl;

	return 0;
}

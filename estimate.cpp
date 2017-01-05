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
} Nsample;

class Testcase {
	public:
		int no;
		char filename[32];
		Time selective_time;
		vector<Nsample> selective_samples;
		Time unselective_time;
		vector<Nsample> unselective_samples;
		Nsample totals;
		Nsample totalu;
		friend ostream& operator << (ostream& out, Testcase tc) {
			out << tc.no << "-<" << tc.filename << ">\n";
			out << "  |-selective-" << tc.totals.randn << "++" << tc.totals.slctn << "\n";
			out << "  |-unselective-" << tc.totalu.randn << "++" << tc.totalu.slctn << "\n";
		}
};

vector<Testcase> testcases;


void parse_statistics(ifstream& inf) {
	string date;
   	getline(inf, date);
	cout << date << endl;
	while (!inf.eof()) {
		Testcase tc;
		tc.totals.randn = 0;
		tc.totals.slctn = 0;
		string tmpstr;
		//char tmpcstr[257];
		//inf >> tc.no;
		//cout << tc.no << endl;
		inf >> tc.filename;
		cout << "filename: " << tc.filename << endl;
		while (true){
			int c = inf.peek();
			if (c < '0' || c > '9') break;
			if (tmpstr.find("real") != string::npos) {
				break;
			}
			Nsample ns;
			inf >> ns.randn >> ns.slctn;
			tc.selective_samples.push_back(ns);
			tc.totals.randn += ns.randn;
			tc.totals.slctn += ns.slctn;
		}
		while (tmpstr != "real")
			inf >> tmpstr;
		inf >> tc.selective_time.real; 
		inf >> tmpstr >> tc.selective_time.user; 
		inf >> tmpstr >> tc.selective_time.sys; 

		inf >> tmpstr;
		while (true){
			int c = inf.peek();
			if (c < '0' || c > '9') break;
			if (tmpstr.find("real") != string::npos) {
				break;
			}
			Nsample ns;
			inf >> ns.randn >> ns.slctn;
			tc.selective_samples.push_back(ns);
			tc.totals.randn += ns.randn;
			tc.totals.slctn += ns.slctn;
		}
		while (tmpstr != "real")
			inf >> tmpstr;
		inf >> tc.unselective_time.real; 
		inf >> tmpstr >> tc.unselective_time.user; 
		inf >> tmpstr >> tc.unselective_time.sys; 

		testcases.push_back(tc);
		inf >> tmpstr;
		inf >> tmpstr;
	}
	return;
}

int main(int argc, char** argv)
{
	ifstream fp(argv[1]);
	parse_statistics(fp);
	fp.close();
	//for (size_t i = 0; i < testcases.size(); i++)
	//	cout << testcases[i];

	return 0;
}

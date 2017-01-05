/***************************************************************
 *  @file estimate.cpp
 *  @brief           
 *             
 *  @author Li Jiaying
 *  @bug no known bugs
 ***************************************************************/
#include <stdio>
#include <vector>
#include <string>
#include <assert>


typedef struct Time_tag {
	float real;
	float user;
	float sys;
} Time;

typedef struct Nsample_tag {
	int randn;
	int slctn;
} Nsample;

typedef struct Testcase_tag {
		int no;
		char filename[32];
		Time selective_time;
		Nsample selective_samples[200];
		int selective_num;
		Time unselective_time;
		Nsample unselective_samples[200];
		int unselective_num;
		Nsample totals;
		Nsample totalu;
} Testcase;

/*
void output (Testcase tc) {
	cout << tc.no << "-<" << tc.filename << ">\n";
	cout << "  |-selective-" << tc.totals.randn << "++" << tc.totals.slctn << "\n";
	cout << "  |-unselective-" << tc.totalu.randn << "++" << tc.totalu.slctn << "\n";
}
*/

Testcase testcases[100];
int cur = 0;


void parse_statistics(FILE* fd) {
	char date[32];
	getline(fd, date, 31);
	while (!fd.eof()) {
		Testcase* tc = testcases[cur];
		tc->totals.randn = 0;
		tc->totals.slctn = 0;
		char str[256];
		//char tmpcstr[257];
		//inf >> tc.no;
		//cout << tc.no << endl;
		getline(fd, tc.filename, 31);
		cout << "filename: " << tc.filename << endl;
		tc->selective_num = 0;
		tc->unselective_num = 0;
		while (true){
			getline(fd, str, 255);
			if (strlen(str) == 0) continue;
			if (str[0] < '0' || str[0] > '9') break;
			Nsample ns;
			sscanf("%d %d", &tc->selective_samples[tc->selective_num].randn, 
					&tc->selective_samples[tc->selective_num].slctn);
			tc->totals.randn += tc->selective_samples[tc->selective_num].randn;
			tc->totals.slctn += tc->selective_samples[tc->selective_num].slctn;
			tc->selective_num++;
		}
		while (strncmp(str, "real", 4) != 0)
			getline(fd, str, 255);
		sscanf(str, "real %d", &tc.selective_time.real); 
		getline(fd, str, 255);
		sscanf(str, "user %d", &tc.selective_time.user); 
		getline(fd, str, 255);
		sscanf(str, "sys %d", &tc.selective_time.sys); 

		while (true){
			getline(fd, str, 255);
			if (strlen(str) == 0) continue;
			if (str[0] < '0' || str[0] > '9') break;
			Nsample ns;
			sscanf("%d %d", &ns.randn, &ns.slctn);
			tc.unselective_samples.push_back(ns);
			tc.totalu.randn += ns.randn;
			tc.totalu.slctn += ns.slctn;
		}
		while (strncmp(str, "real", 4) != 0)
			getline(fd, str, 255);
		sscanf(str, "real %d", &tc.unselective_time.real); 
		getline(fd, str, 255);
		sscanf(str, "user %d", &tc.unselective_time.user); 
		getline(fd, str, 255);
		sscanf(str, "sys %d", &tc.unselective_time.sys); 

		testcases.push_back(tc);
	}
	return;
}

int main(int argc, char** argv)
{
	FILE* fp;
	fp = fopen(argv[1], "r+");
	parse_statistics(fp);
	fclose(fp);
	//for (size_t i = 0; i < testcases.size(); i++)
	//	cout << testcases[i];

	return 0;
}

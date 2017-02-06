/***************************************************************
 *  @file main.cpp
 *  @brief           
 *             
 *  @author Li Jiaying
 *  @bug no known bugs
 ***************************************************************/
#include <fstream>
#include <string>
#include <cassert>
#include <getopt.h>
#include "node.hpp"
#include "parser.hpp"
#include "loop_handler.hpp"
using namespace std;

#define RED "\e[31m"
#define YELLOW "\e[33m"
#define GREEN "\e[32m"
#define BLUE "\e[34m"
#define NORMAL "\e[0m"
#define BOLD "\e[1m"
#define UNDERLINE "\e[4m"

Loop* loop = new Loop();
int type = -1;
int branch = -1;
char *outputfile = NULL, *cefile = NULL, *pcfile = NULL, *varfile = NULL, *invfile = NULL, *nvnb_file = NULL;
bool outputNvNb = false;
bool outputVarfile = false;
int property_choice = 0;

struct option longopts[] = {
	//{"type", required_argument, NULL, 't'},
	{"type", required_argument, NULL, 't'},
	{"output", required_argument, NULL, 'o'},
	{"cefile", required_argument, NULL, 'c'},
	{"pcfile", required_argument, NULL, 'm'},
	{"varfile", required_argument, NULL, 'v'},
	{"invfile", required_argument, NULL, 'i'},
	{"branch", required_argument, NULL, 'b'},
	{"properity", required_argument, NULL, 'p'},
	{0, 0, 0, 0},
};


int main(int argc, char** argv)
{
	extern int yyparse(void);
	extern FILE *yyin;

	int c;

	while((c = getopt_long(argc, argv,"t:o:c:m:v:i:b:N:Vp:", longopts, NULL)) != -1){
		switch (c){
			case 't':
				//cout << GREEN << " type: " << NORMAL << optarg; 
				type = atoi(optarg);
				break;
			case 'o':
				//cout << GREEN << " outputfile: " << NORMAL << optarg; 
				outputfile = optarg;
				break;
			case 'c':
				//cout << GREEN << " counterexamplefile: " << NORMAL << optarg; 
				cefile = optarg;
				break;
			case 'm':
				//cout << GREEN << " counterexamplefile: " << NORMAL << optarg; 
				pcfile = optarg;
				break;
			case 'v':
				//cout << GREEN << " varfile: " << NORMAL << optarg; 
				if (varfile == NULL)
					varfile = optarg;
				break;
			case 'i':
				//cout << GREEN << " invfile: " << NORMAL << optarg; 
				invfile = optarg;
				break;
			case 'b':
				//cout << GREEN << " branchReverse: " << NORMAL << optarg; 
				branch = atoi(optarg);
				break;
			case 'N':
				//cout << GREEN << " nvnb_file: " << NORMAL << optarg; 
				outputNvNb = true;
				nvnb_file = optarg;
				break;
			case 'V':
				outputVarfile = true;
				if (varfile == NULL)
					varfile = optarg;
				break;
			case 'p':
				property_choice = atoi(optarg);
				break;
		}
	}


	if (type == -1) {
		cerr << RED << "The executable need more arguments." << endl;
		cerr << BLUE << "<USAGE> parser -t type [option]\n";
		cerr << "[option]:\n"; 
		cerr << "\t-c, --cefile filename\n";
		cerr << "\t-m, --pcfile filename\n";
		cerr << "\t-v, --varfile filename\n";
		cerr << "\t-i, --invfile filename\n";
		cerr << "\t-N: output Nv_Nb file\n";
		cerr << "\t-V: output Var file\n";
		cerr << "\t-p: property_choice[1-3] used for verification\n";
		cerr << NORMAL;
		cout << endl;
		return -1;
	}

	//cout << BLUE << "parsing...................\n" << NORMAL;
	yyin = stdin;
	if (yyparse()) {
		fprintf(stderr, "Error ! Error ! Error !\n");
		exit(1);
	}
	loop->adjust();
	//cout << BLUE << "AST:\n";
	//cout << BLUE;
	//loop->print();
	//cout << NORMAL;
	LoopHandler* lh = new LoopHandler(loop);
	ofstream ofs(outputfile);
	switch (type) {
		case 0:
		// plain c file for user reference
			ofs << lh->toCFile() << endl;
			break;
		case 1:
			// previous setting, only record pass/fail to Pre/Post
			ofs << lh->toCFileWithLoopRecord(cefile, varfile, invfile, pcfile) << endl;
			break;
		case 2:
			// used for current setting to generate file to RUN ON
			ofs << lh->toCFileWithLoopBranchesRecord(cefile, varfile, invfile, pcfile) << endl;
			break;
		case 3:
			// file used for our system to run "specific inputs" on, selective sampling
			assert(branch>=0 && branch<loop->loopbody->n);
			ofs << lh->toReverseBranchFile4Klee(branch);
			break;
		case 4:
			// file used for our system to run "specific inputs" on, selective sampling
			//cout << "come to case 4\n";
			assert(property_choice > 0 && property_choice <= 3);
			//cout << "after assertion\n";
			ofs << lh->toCFile4Verification(property_choice);
			//cout << "after to c file\n";
			break;
	}
	ofs.close();
	delete lh;
	if (outputNvNb) {
		ofstream ofs2(nvnb_file);
		//ofs2 << "set(Nv " << loop->names->n << ")\n";
		//ofs2 << "set(Nb " << (loop->loopbody->n>0? loop->loopbody->n:1) << ")\n";
		ofs2 << loop->names->n << " " << loop->loopbody->n << "\n";
		ofs2.close();
	}
	if (outputVarfile) {
		ofstream ofs3(varfile);
		ofs3 << loop->names->n << "\n";
		for (int i = 0; i < loop->names->n; i++)
			ofs3 << loop->names->u[i] << "\t";
		ofs3.close();
	}
	//return loop->names->n;
	return 0;
}

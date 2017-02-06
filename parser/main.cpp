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
char *outputfile = NULL, *varfile = NULL, *invfile = NULL, *cefile = NULL, *dsfile = NULL;
int choice = 0;

struct option longopts[] = {
	{"type", required_argument, NULL, 't'},
	{"output", required_argument, NULL, 'o'},
	{"varfile", required_argument, NULL, 'v'},
	{"invfile", required_argument, NULL, 'i'},
	{"cefile", required_argument, NULL, 'x'},
	{"dsfile", required_argument, NULL, 'd'},
	{"choice", required_argument, NULL, 'c'},
	{0, 0, 0, 0},
};


int main(int argc, char** argv)
{
	extern int yyparse(void);
	extern FILE *yyin;

	int c;

	while((c = getopt_long(argc, argv,"t:o:c:i:x:d:v:", longopts, NULL)) != -1){
		switch (c){
			case 't': //cout << GREEN << " type: " << NORMAL << optarg; 
				type = atoi(optarg);
				break;
			case 'o': //cout << GREEN << " outputfile: " << NORMAL << optarg; 
				outputfile = optarg;
				break;
			case 'c': //cout << GREEN << " counterexamplefile: " << NORMAL << optarg; 
				choice = atoi(optarg);
				break;
			case 'i': 
				invfile = optarg;
				break;
			case 'x': 
				cefile = optarg;
				break;
			case 'd': 
				dsfile = optarg;
				break;
			case 'v': //cout << GREEN << " varfile: " << NORMAL << optarg; 
				if (varfile == NULL)
					varfile = optarg;
				break;
		}
	}


	if (type == -1) {
		cerr << RED << "The executable need more arguments." << endl;
		cerr << BLUE << "<USAGE> parser -t type [option]\n";
		cerr << "[option]:\n"; 
		cerr << "\t-o, --output : output filename\n";
		cerr << "\t-v, --varfile : var filename\n";
		cerr << "\t-i, --invfile : invariant filename PREFIX\n";
		cerr << "\t-x, --cefile : counter-example filename\n";
		cerr << "\t-d, --dsfile : dataset filename\n";
		cerr << "\t-c: --choice : verification_choice[1-3] used for verification// generating positive/negative init samples when type = 5\n";
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
	//cout << BLUE << "AST:\n";
	//cout << BLUE;
	//loop->print();
	//cout << NORMAL;
	LoopHandler* lh = new LoopHandler(loop);
	ofstream ofs(outputfile);
	switch (type) {
		case 0: // plain c file for user reference
			ofs << lh->toCFile() << endl;
			break;
		case 1: // previous setting, only record pass/fail to Pre/Post
			ofs << lh->toCFileWithLoopRecord(cefile, varfile, invfile, dsfile) << endl;
			break;
		case 2: // used for current setting to generate file to RUN ON
			ofs << lh->toCFileWithLoopBranchesRecord() << endl;
			break;
		case 3: // file used for our system to run "specific inputs" on, selective sampling
			ofs << lh->toReverseBranchFile4Klee();
			break;
		case 4: // file used for our system to run "specific inputs" on, selective sampling
			assert(choice > 0 && choice <= 3);
			ofs << lh->toCFile4Verification(choice);
			break;
		case 5: // file used for our system to run "specific inputs" on, selective sampling
			//cout << "--->toCFile4Init with CHOICE=" << choice << "\n";
			ofs << lh->toCFile4Init(choice);
			//cout << "<---toCFile4Init with CHOICE=" << choice << "\n";
			break;
	}
	ofs.close();
	delete lh;

	ofstream ofs3(varfile);
	ofs3 << loop->names->n << "\n";
	for (int i = 0; i < loop->names->n; i++)
		ofs3 << loop->names->u[i] << "\t";
	ofs3.close();
	if (type == 1)
		return loop->names->n;
	return 0;
}

/***************************************************************
 *  @file model_parser.cpp
 *  @brief           
 *             
 *  @author Li Jiaying
 *  @bug no known bugs
 ***************************************************************/
#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
using namespace std;

class Model_Var{
	public:
		string name;
		int value;
		friend ostream& operator << (ostream& out, const Model_Var& mv) {
			out << mv.value; 
			return out;
		}
};

Model_Var* model;
int Nv = 0;

bool save_to_file(const char* filename) {
	ofstream ofs(filename);
	for (int i = 0; i < Nv; i++) {
		ofs << model[i].value << "\t";
	}
	ofs << endl;
	ofs.close();
	return true;
}

bool parse_var_file(const char* varfile) {
	//cout << "parsing var file\n";
	ifstream inf(varfile);
	inf >> Nv;
	model = new Model_Var[Nv];
	for (int i = 0; i < Nv; i++)
		inf >> model[i].name;
	//for (int i = 0; i < Nv; i++)
	//	cout << model[i].name << endl;;
	inf.close();
	return true;
}


int main(int argc, char** argv) {
	assert ((argc < 2) || "# of arguments is less than 2.");
	parse_var_file(argv[1]);
	string name;
	int value;
	//cout << "parsing input...\n";
	cin >> name;
	//cout << name << endl;
	if (name.find("unsat", 0) != string::npos) return 0;
	for(int i = 0; i < Nv; i++) {
		cin >> name >> value;
		//cout << name << " -> " << value << endl;
		for (int j = 0; j < Nv; j++) {
			if (name == model[j].name) {
				model[j].value = value;
				break;
			}
		}
	}

	/*cout << "-->model:\n";
	for (int j = 0; j < Nv; j++)
		cout << model[j] << "\t";
	cout << endl;
	*/
	if (argc > 2)
		save_to_file(argv[2]);

	// sat
	return 1;
}

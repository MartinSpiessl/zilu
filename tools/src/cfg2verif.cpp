/*
 *  parameter: cfgfilepath 文件的绝对路径名如: /user/home/my.cfg
 *  key         文本中的变量名
 *  value       对应变量的值，用于保存
 *  
 */
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;

const int max_confignum = 32;
enum category {NAME=0, BEFL, BEFLI, SYM, PREC, LOOPC, LOOP, POSTC, AFTL, INV, LEARNERS};

class Config {
	public:
		string key;
		string value;
		string cppstatement;

		friend std::ostream& operator << (std::ostream& out, const Config& c) {
			out << c.key << " = " << c.value;
			return out;
		}

		/*
		bool toCppStatement() {
			//cout << "processing <" + key + ", " + value + ">......\n";
			if (key == "precondition") { cppstatement = "iif_assume(" + value + ");";
			} else if (key == "beforeloop") { cppstatement = value;
			} else if (key == "beforeloopinit") { cppstatement = value;
			} else if (key == "loop") { cppstatement = value;
			} else if (key == "loopcondition") { cppstatement = "while(" + value + ")";
			} else if (key == "loop") { cppstatement = value;
			} else if (key == "postcondition") { 
				cppstatement = "iif_assert(" + value + ");\n"; 
				cppstatement += "return 0;\n}";
			} else if (key == "afterloop") { cppstatement = value;
			}
			return true;
		}
		*/
};


class FileHelper {
	public:
		FileHelper(const char* cfgfname, const char* cppfname) {
			this->cfgfilename = cfgfname;

			int len = strlen(cppfname);
			cppfilename = new char[len + 10];
			strcpy(cppfilename, cppfname);
			cppfilename[len-2] = '\0';
			strcat(cppfilename, "_klee0.c");

			cs = new Config[max_confignum];
			int i = 0;
			cs[i++].key = "names";
			cs[i++].key = "beforeloop";
			cs[i++].key = "beforeloopinit";
			cs[i++].key = "symbolic";
			cs[i++].key = "precondition";
			cs[i++].key = "loopcondition";
			cs[i++].key = "loop";
			cs[i++].key = "postcondition";
			cs[i++].key = "afterloop";
			cs[i++].key = "invariant";
			cs[i++].key = "learners";
			confignum = i;
			for (int i = 0; i < confignum; i++)
				cs[i].value= string("");
			//variables = NULL;
			vnum = 0;
		}

		~FileHelper() {
			if (cs != NULL)
				delete []cs;
			variables.clear();
		}

		bool readConfigFile() {
			ifstream cfgFile(cfgfilename);
			if(!cfgFile.is_open()) {
				cout<<"can not open cfg file!"<<endl;
				return false;
			}
			string line;
			int lastidx = -1;
			while(getline(cfgFile, line)) {
				size_t pos = line.find('=');
				/*if(pos == string::npos) {
					if (lastidx >= 0) 
						cs[lastidx].value += "\n" + line;
					continue;
				}*/
				string key = line.substr(0,pos);
				bool get_record = false;
				for (int i = 0; i < confignum; i++) {
					if(cs[i].key == key) {
						get_record = true;
						cs[i].value += line.substr(pos+1);
						lastidx = i;
						break;
					}
				}
				if (get_record == true)
					continue;
				else 
					//cs[lastidx].value += line;
					cs[lastidx].value += "\n" + line;
			}

			cfgFile.close();

			size_t start = 0;
			size_t end = cs[NAME].value.find(' ');
			while (end == start) {
				end = cs[NAME].value.find(' ', end+1);
				start++;
			}
			for (int i = 0; end != std::string::npos; i++) {
				variables.push_back(cs[NAME].value.substr(start, end-start));
				start = end + 1;
				end = cs[NAME].value.find(' ', start);
			}
			variables.push_back(cs[NAME].value.substr(start, end-start));
			vnum = variables.size();

			return true;
		}

		bool writeCFile() {
			int len = strlen(cppfilename);
			cppfilename[len-3] = '0';

			for (int choice = 1; choice <= 3; choice++) {
				cppfilename[len-3] += 1;
				//std::cout << "choice" << choice << " : " << cppfilename << std::endl;
				std::ofstream cppFile(cppfilename);
				if(!cppFile.is_open()) {
					cout<<"can not open cpp file!"<<endl;
					return false;
				}
				writeHeader(cppFile);
				writeMain(cppFile, choice);
				cppFile.close();
			}
			return true;
		}

		inline int getVnum() {
			return vnum;
		}


	private:
		inline bool writeHeader(ofstream& cppFile) {
			cppFile << "#include <klee/klee.h>\n" << endl;
			return true;
		}

		inline bool writeMain(ofstream& cppFile, int choice) {
			cppFile << "\nint main() {\n"; 
			for (int i = 0; i < vnum; i++) 
				cppFile << "int " + variables[i] + ";\n";
			for (int i = 0; i < vnum; i++) 
				cppFile << "klee_make_symbolic(&" << variables[i] <<", sizeof(" << variables[i] << "), \"" << variables[i] << "\");\n";
			if (cs[SYM].value.compare("") != 0) {
				cppFile << "int " + cs[SYM].value + ";\n";
				cppFile << "klee_make_symbolic(&" << cs[SYM].value <<", sizeof(" << cs[SYM].value << "), \"" << cs[SYM].value << "\");\n";
			}
			// before loop statements;
			if (cs[BEFL].value.compare("") != 0)
				cppFile << cs[BEFL].value << std::endl;

			switch (choice) {
				case 1:
					if (cs[BEFLI].value.compare("") != 0)
						cppFile << cs[BEFLI].value << std::endl;
					cppFile << "klee_assume(" << cs[PREC].value <<");\n"; 
					cppFile << "klee_Massert(" << cs[INV].value <<");\n"; 
					break;
				case 2:
					if (cs[BEFLI].value.compare("") != 0)
						cppFile << cs[BEFLI].value << std::endl;
					if (cs[LOOPC].value.compare("") != 0)
						cppFile << "klee_assume(" << cs[LOOPC].value <<");\n"; 
					cppFile << "klee_assume(" << cs[INV].value <<");\n"; 
					cppFile << "do {\n";
					cppFile << cs[LOOP].value << "\n";
					cppFile << "} while(0);\n";
					cppFile << "klee_Massert(" << cs[INV].value <<");\n"; 
					break;
				case 3:
					if (cs[LOOPC].value.compare("") != 0)
						cppFile << "klee_assume(!(" << cs[LOOPC].value <<"));\n"; 
					cppFile << "klee_assume(" << cs[INV].value <<");\n"; 
					cppFile << "klee_Massert(" << cs[POSTC].value <<");\n"; 
					break;
			}
			cppFile << "return 0;\n}" << endl;
			return true;
		}

	private:
		const char* cfgfilename;
		char* cppfilename;
		Config* cs;
		int confignum;
		vector<string> variables;
		int vnum;
};


int main(int argc, char** argv) 
{
	const char* cfgfilename = "inputcfg.cfg";
	const char* cppfilename = "inputcfg.c";
	if (argc >= 2) cfgfilename = argv[1];
	if (argc >= 3) cppfilename = argv[2];
	FileHelper fh(cfgfilename, cppfilename);
	fh.readConfigFile();
	fh.writeCFile();
	return fh.getVnum();
}

#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

const int max_confignum = 32;

class Config {
	public:
		string key;
		string value;
		string cppstatement;

		friend std::ostream& operator << (std::ostream& out, const Config& c) {
			out << c.key << " = " << c.value;
			return out;
		}

		bool toCppStatement() {
			//cout << "processing <" + key + ", " + value + ">......\n";
			if (key == "precondition") { cppstatement = "iif_assume(" + value + ");";
			} else if (key == "beforeloop") { cppstatement = value;
			} else if (key == "beforeloopinit") { cppstatement = value;
			} else if (key == "symbolic") { 
				if (value.compare("") != 0) 
					cppstatement = value;
				else 
					cppstatement = "";
			} else if (key == "loop") { cppstatement = value;
			} else if (key == "loopcondition") { 
				if (value.compare("") == 0) 
					cppstatement = "while(rand() % 8)";
				else
					cppstatement = "while(" + value + ")";
			} else if (key == "loop") { cppstatement = value;
			} else if (key == "postcondition") { 
				cppstatement = "iif_assert(" + value + ");\n"; 
				cppstatement += "return 0;\n}";
			}
			return true;
		}
};


class FileHelper {
	public:
		FileHelper(const char* cfgfilename, const char* cppfilename, const char* varfilename, const char* invfileprefix, const char* testcasefilename, const char* oldtracefilename) {
			this->cfgfilename = cfgfilename;
			this->cppfilename = cppfilename;
			this->varfilename = varfilename;
			this->invfileprefix = invfileprefix;
			this->testcasefilename = testcasefilename;
			this->oldtracefilename = oldtracefilename;
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
			cs[i++].key = "learners";
			confignum = i;
			vnum = 0;
		}

		~FileHelper() {
			if (cs != NULL)
				delete []cs;
			variables.clear();
			learners.clear();
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
					cs[lastidx].value += "\n" + line;
			}
			cfgFile.close();

			size_t start = 0;
			size_t end = cs[0].value.find(' ');
			while (end == start) {
				end = cs[0].value.find(' ', end+1);
				start++;
			}
			for (int i = 0; end != std::string::npos; i++) {
				variables.push_back(cs[0].value.substr(start, end-start));
				start = end + 1;
				end = cs[0].value.find(' ', start);
			}
			if (start != end)
				variables.push_back(cs[0].value.substr(start, end-start));
			vnum = variables.size();

			int index = confignum - 1;
			start = 0;
			end = cs[index].value.find(' ');
			while (end == start) {
				end = cs[index].value.find(' ', end+1);
				start++;
			}
			for (int i = 0; end != std::string::npos; i++) {
				learners.push_back(cs[index].value.substr(start, end-start));
				start = end + 1;
				end = cs[index].value.find(' ', start);
			}
			if (start != end)
				learners.push_back(cs[index].value.substr(start, end-start));

			return true;
		}

		bool writeCppFile() {
			for (int i = 0; i < confignum; i++)
				cs[i].toCppStatement();
			ofstream cppFile(cppfilename);
			if( !cppFile.is_open()) {
				cout<<"can not open cpp file!"<<endl;
				return false;
			}
			writeCppHeader(cppFile);
			writeCppLoopFunction(cppFile);
			//cppFile << endl;
			writeCppMain(cppFile);
			cppFile.close();
			return true;
		}

		inline int getVnum() {
			return vnum;
		}

		bool writeVarFile() {
			ofstream varFile(varfilename);
			if(!varFile.is_open()) {
				cout<<"can not open log file!"<<endl;
				return false;
			}
			varFile << vnum << endl;
			for (int i = 0; i< vnum; i++)
				varFile << variables[i] << endl;
			varFile.close();
			return true;
		}

	private:
		inline bool writeRecordi(ofstream& cppFile) {
			cppFile << "recordi(" << variables[0];
			for (int i = 1; i < vnum; i++)
				cppFile << ", " << variables[i];
			cppFile << ");";
			return true;
		}

		inline bool writeCppHeader(ofstream& cppFile) {
			cppFile << "#include \"iif.h\"\n"
				<< "#include <iostream>\n" 
				<< "using namespace iif;\n\n";
			return true;
		}

		bool writeCppLoopFunction(ofstream& cppFile) {
			cppFile <<"int loopFunction(int _reserved_input_[]) {\n";
			for (int i = 0; i < vnum; i++) {
				cppFile << "int " << variables[i] << " = _reserved_input_[" << i << "];\n";
			}
			cppFile << "\n";
			//bool inloop = false;
			int symb = -1;
			for (int i = 0; i < confignum; i++) {
				if (cs[i].key == "loop") { 
					cppFile << "{\n"; 
					writeRecordi(cppFile); 
					cppFile << "\n"; 
					//inloop = true;
				}
				if (cs[i].key == "symbolic") {
					if (cs[i].value.compare("") == 0) continue;
					symb = i;
					//if (!inloop) 
					cppFile << "int " << cs[i].cppstatement << " = rand()%2;\n";
					continue;
				}
				if (cs[i].cppstatement.compare("") != 0)
					cppFile << cs[i].cppstatement << endl;
				if (cs[i].key == "loop") { 
					if (symb >= 0) cppFile << cs[symb].cppstatement << " = rand()%2;\n";
					cppFile << "}\n"; 
					writeRecordi(cppFile); 
					cppFile << "\n"; 
				}
			}
			return true;
		}

		inline bool writeCppMain(ofstream& cppFile) {
			cppFile << "\nint main(int argc, char** argv)\n {\n";
			if (oldtracefilename)
				cppFile << "iifContext context(\"" << varfilename 
					<<"\", loopFunction, \"loopFunction\", \"" << oldtracefilename << "\");\n";
			else
				cppFile << "iifContext context(" << varfilename <<"\", loopFunction, \"loopFunction\");\n";
			/*
			if (oldtracefilename)
				cppFile << "iifContext context(\"../" << varfilename 
					<<"\", loopFunction, \"loopFunction\", \"../" << oldtracefilename << "\");\n";
			else
				cppFile << "iifContext context(\"../" << varfilename <<"\", loopFunction, \"loopFunction\");\n";
				*/

			{
				/*std::cout << "size=" << learners.size() << std::endl;
				for (int i = 0; i < learners.size(); i++) {
					std::cout << "-|-> \"" << learners[i] << "\" ";
				}*/
				
				std::cout << "Learners [["; // "size=" << learners.size() << std::endl;
				int size = learners.size();
				if (size == 0 || learners[0].compare("") == 0 || learners[0].find("def") != string::npos) {
					std::cout << "--> linear --> polynomial --> conjunctive";
					cppFile << "context.addLearner(\"linear\");\n";
					cppFile << "context.addLearner(\"poly\");\n";
					cppFile << "context.addLearner(\"conjunctive\");\n";
				} else {
					for (int i = 0; i < size; i++) {
						if (learners[i].find("lin") != string::npos) {
							std::cout << "--> linear";
							cppFile << "context.addLearner(\"linear\");\n";
						} else if (learners[i].find("poly") != string::npos) {
							std::cout << "--> polynomial";
							cppFile << "context.addLearner(\"poly\");\n";
						} else if (learners[i].find("conj") != string::npos) {
							std::cout << "--> conjunctive";
							cppFile << "context.addLearner(\"conjunctive\");\n";
						}
					}
				}
				std::cout << "]]";
			}

			if (testcasefilename) {
				cppFile << "return context.learn(\"" << testcasefilename << "\", \"" << invfileprefix << "\");\n}" << endl;
			} else {
				cppFile << "return context.learn(NULL, \"" << invfileprefix << "\");\n}" << endl;
			}
			/*
			if (testcasefilename) {
				cppFile << "return context.learn(\"../" << testcasefilename << "\", \"../" << invfileprefix << "\");\n}" << endl;
			} else {
				cppFile << "return context.learn(NULL, \"../" << invfileprefix << "\");\n}" << endl;
			}
			*/
			return true;

			/*cppFile << "int main(int argc, char** argv)\n {\n" 
				<< "iifContext context(\"../" << varfilename <<"\", loopFunction, \"loopFunction\");\n"
				<< "context.addLearner(\"poly\");\n"
				<< "context.addLearner(\"rbf\");\n"
				<< "context.addLearner(\"linear\").addLearner(\"rbf\");\n"
				<< "context.addLearner(\"linear\").addLearner(\"conjunctive\");\n"
				<< "return context.learn(\"../" << invfileprefix << "\");\n}" << endl;
				*/
		}

	private:
		const char* cfgfilename;
		const char* cppfilename;
		const char* varfilename;
		const char* invfileprefix;
		const char* testcasefilename;
		const char* oldtracefilename;
		Config* cs;
		int confignum;
		vector<string> variables;
		vector<string> learners;
		int vnum;
};


int main(int argc, char** argv) 
{
	const char* cfgfilename = "inputcfg.cfg";
	const char* cppfilename = "inputcfg.cpp";
	const char* varfilename = "inputcfg.var";
	const char* invfileprefix = "inputcfg";
	const char* testcasefilename = NULL;
	const char* oldtracefilename = NULL;
	if (argc >= 2) cfgfilename = argv[1];
	if (argc >= 3) cppfilename = argv[2];
	if (argc >= 4) varfilename = argv[3];
	if (argc >= 5) invfileprefix = argv[4];
	if (argc >= 6) testcasefilename = argv[5];
	if (argc >= 7) oldtracefilename = argv[6];
	//cout << "!!!" << testcasefilename << " !!!" << oldtracefilename << endl;

	FileHelper fh(cfgfilename, cppfilename, varfilename, invfileprefix, testcasefilename, oldtracefilename);
	//cout << "after construct...\n";
	fh.readConfigFile();
	//cout << "after read config file...\n";
	fh.writeCppFile();
	//cout << "after write cpp file...\n";
	fh.writeVarFile();
	//cout << "after write var file...\n";
	//fh.writeInvFile();
	return fh.getVnum();
}

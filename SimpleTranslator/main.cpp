#include "header.h"

using namespace std;

int main(int argc, char const *argv[])
{
	ifstream inputFile;
	LexemManager lexemAnalysis;

	try {
		if (argv[1] == NULL) {
			throw "no input file!";
		}

		inputFile.open(argv[1], ios::in);

		// Check is file can be opened?
		if (!inputFile.is_open()) { 
			throw "no such file!";
		}

		lexemAnalysis.doLexemRoutine(inputFile);

		inputFile.close();

		lexemAnalysis.doSyntaxRoutine();

		//lexemAnalysis.printTable();
	}
	
	catch (const char* err) {
		cerr << COLOR_RED << "Error" << COLOR_OFF << ": ";
		cerr << err << endl;
		return -1;
	}
	catch (...) {
		cerr << COLOR_RED << "Error" << COLOR_OFF << ": ";
		cerr << "something wrong" << endl;
		return -1;
	}
	
	return 0;
}
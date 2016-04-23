#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>

#include <algorithm>

#include <utility>      // std::pair

#include <vector>
#include <stack>
#include <map>

#define COLOR_RED "\033[31m" // red
#define COLOR_GRN "\033[32m" // green
#define COLOR_YEL "\033[33m" // yellow
#define COLOR_BLU "\033[34m" // blue
#define COLOR_MAG "\033[35m" // magneta
#define COLOR_CYN "\033[36m" // cyan
#define COLOR_WHT "\033[37m" // white
#define COLOR_OFF "\033[0m"	 // reset

// Describes states of the finite state machine
enum States {	
				START, 		// Start point   : start of the file
				DATATYPE, 	// Data type     : int, char
				DELIMETR, 	// Delimert      : ','
				ENDSTR, 	// End of string : '.'
				VARIABLE, 	// Variable      : 'A'-'Z'
				EXIT, 		// Exit          : exit from the program (return)
				COMMAND,  	// Command       : (printf) write, (scanf) read
				OPERATION,  // Operation     : >=, <=, ==, >, <, !=
				MATH, 		// Mathematics   : =, +, -, *, /
				ASIGN, 		// Asign 		 : ':'
				IF, 		// If 			 : if open tag
				ENDIF, 		// End if 		 : if close tag
				ELSE, 		// Else 		 : else open tag 
				ENDELSE, 	// End else 	 : else close tage
				LOOP, 		// Loop 		 : loop open tag
				ENDLOOP, 	// End loop 	 : loop close tag
				ERROR, 		// Error 		 : lexem error
				END 		// End point  	 : end of the file
			};

// Describes syntax node stuff
class SyntaxNode
{
	private:
		int i_;// what we are input
		int j_;// what we are already store
		int s_;// state(1/2-push/pop)

	public:
		SyntaxNode(int i, int j, int s);
		~SyntaxNode();

		int getI();
		int getJ();
		int getS();
};

// Describes lexem node stuff
class LexemNode {
	private:
		enum States lexemNodeState_;
		int 		lexemNodeLine_;
		int 		lexemNodePosition_;
		std::string lexemNodeValue_;
		std::string lexemNodeDataType_;

	public:
		LexemNode(enum States newState, std::string newValue, int newLine,\
															int newPostion);
		LexemNode(enum States newState, std::string newValue, int newLine,\
									int newPostion, std::string newDataType);
		~LexemNode();

		int getLine(); 
		int getPosition(); 
		enum States getState();
		std::string getType();
		std::string getValue();
};

// Stores lexem nodes
class LexemTable {
	private:
		std::vector<LexemNode> lexemTable_;

	public:
		LexemTable();
		~LexemTable();

		void addToTable(enum States state, std::string value, int line,\
											int position, std::string type);
		std::vector<LexemNode> getTable();

		void printTable();
};

// Describes varibale and their data types
class DataTypeManager
{
	private:
		std::map<std::string, std::string> dataTypeMap_;
	public:
		DataTypeManager();
		~DataTypeManager();

		std::string getDataType(std::string variable, std::string type);
};

class SyntaxManager
{
	private:
		std::vector<SyntaxNode>* syntaxMatrix_;
		std::stack<std::pair<States, std::string>>* syntaxStack_;

	public:
		SyntaxManager();
		~SyntaxManager();	
		
		void doSyntaxRoutine(LexemTable* table);
		int syntaxAnalysis(std::vector<LexemNode> table);

		void checkLoopIf(std::vector<LexemNode> table);

		int searchSyntaxMatrix(int j, int i);
		int addSyntaxMatrix(int i, int j, int s);
		void fillSyntaxMatrix();

		bool stack_empty();
		int stack_pop();
		int stack_push(enum States S, std::string V);
		enum States get_head_stack_state();
		std::string get_head_stack_value();
		void compare_states(int input, enum States S, std::string V);
};

// Do actual work
class LexemManager {
	private:
		LexemTable* lexemTable_;
		SyntaxManager* syntaxManager_;
		DataTypeManager* dataTypeTable_;

	public:
		LexemManager();
		~LexemManager();

		void doLexemRoutine(std::ifstream &inputFile);
		std::string checkDataType(std::string buffer, std::string dataFlag);
		void addToTable(enum States state, std::string value, int line,\
											int position, std::string type);
		void doSyntaxRoutine();
		void printTable();
};

class AssemblyGen
{
	public:
		AssemblyGen();
		~AssemblyGen();
	
	void generateAssembly(std::vector<LexemNode> table);
};

#endif
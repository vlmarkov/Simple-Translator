#include "header.h"

using namespace std;

// Class LexemManager
LexemManager::LexemManager() 
{
	// Create lexem table
	lexemTable_ = new LexemTable;
    syntaxManager_ = new SyntaxManager;
	dataTypeTable_ = new DataTypeManager;
}

LexemManager::~LexemManager() 
{
	// delete whole table
	// call destructor of the LexemTable class
	//cout << "destructor" << endl;
    lexemTable_->~LexemTable();
    syntaxManager_->~SyntaxManager();
}

void LexemManager::doLexemRoutine(ifstream &inputFile) 
{
	enum States CS;

    string buffer;
    string dataFlag;

	int countPosition = 0;
	int countLines = 1;
    
    CS = START; // Current state = Start point

  	while (!inputFile.eof()) {
  		char c;

  		switch(CS)
  		{
  			case START:

  				inputFile.get(c);
  				countPosition++;

  				// Ignoring spaces
		        while (c == ' ' || c == '\t' || c == '\n') { // read while nothing
		        	if (c == '\n') {
		        		countLines++;
		        		countPosition = 0;
		        	}

		        	inputFile.get(c);
		        	countPosition++;
		        }
		  		
		  		// Ignoring comments
		        if (c == '{') {
		            inputFile.get(c);
		            countPosition++;

		            while (c != '}') {
		            	inputFile.get(c);
		            	countPosition++;

		            	if (c == '\n') {
		        			countLines++;
		        			countPosition = 0;
		        		}

		            	if (inputFile.eof()) {
		            		buffer = "not closed tag for comment";
		            		CS = ERROR;
		            		break;
		            	} else if (c == '{') {
		            		buffer = "not closed tag for comment";
		            		CS = ERROR;
		            		break;
		            	}
		            }
        		// For variable
		        } else if (c == '}') {
		       		buffer = "not closed tag for comment";
		       		CS = ERROR;
                } else if (c >= 'A' && c <= 'Z') {
                    CS = VARIABLE;
                }	else if (c == ':') {
                    CS = ASIGN;
                } else if (c == ',') {
                    CS = DELIMETR;
                } else if (c == '.') {
                	CS = ENDSTR;
                } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                	CS = MATH;
                } else if (c == '>' || c == '<' || c == '!') {
                	buffer.push_back(c); // converting 1 char to string
                	inputFile.get(c);
                	countPosition++;

                	switch (c) 
                	{
                		case '=':
                			buffer.push_back(c); // converting 1 char to string
                			CS = OPERATION;
                			break;

                		case ' ':
                			CS = OPERATION;
                			break;

                		default:
                			CS = ERROR;
                			break;
                	}
                } else if (c == '=') {
                	buffer.push_back(c); // converting 1 char to string
                	inputFile.get(c);
                	countPosition++;

                	switch (c)
                	{
                		case '=':
                			buffer.push_back(c); // converting 1 char to string
                			CS = OPERATION;
                			break;

                		case ' ':
                			CS = MATH;
                			break;

                		default:
                			CS = ERROR;
                			break;
                	}
                } else if (c >= 'a' && c <= 'z') {
                	while (!inputFile.eof()) {
                		if ((c == ':') || (c == '.') || (c == '\n') || (c == ' ') || (c == ',')) {
                			break;
                		}
                		buffer.push_back(c); // converting 1 char to string
                		inputFile.get(c);
                		countPosition++;
                	}

                	inputFile.unget(); // reverse one char
                	countPosition--;

                	if (buffer.compare("int") == 0) {
                		dataFlag = "int";
                		CS = DATATYPE;
                	} else if (buffer.compare("char") == 0) {
                		dataFlag = "char";
                		CS = DATATYPE;
                	} else if (buffer.compare("quit") == 0) {
                		CS = EXIT;
                	} else if (buffer.compare("read") == 0) {
                		CS = COMMAND;
                	} else if (buffer.compare("write") == 0) {
                		CS = COMMAND;
                	} else if (buffer.compare("if") == 0) {
                		CS = IF;
                	} else if (buffer.compare("endif") == 0) {
                		CS = ENDIF;
                	} else if (buffer.compare("else") == 0) {
                		CS = ELSE;
                	} else if (buffer.compare("endelse") == 0) {
                		CS = ENDELSE;
                	} else if (buffer.compare("loop") == 0) {
                		CS = LOOP;
                	} else if (buffer.compare("endloop") == 0) {
                		CS = ENDLOOP;
                	} else {
                		CS = ERROR;
                	}
                }
  				break;

  			case DATATYPE:
  				this->addToTable(DATATYPE, buffer, countLines, countPosition, dataFlag);
  				buffer.clear(); // cleaning buffer values
                CS = START;
                break;

  			case VARIABLE:
  				buffer.push_back(c); // converting 1 char to string
  				
  				dataFlag = this->checkDataType(buffer, dataFlag);
  				this->addToTable(VARIABLE, buffer, countLines, countPosition, dataFlag);

  				buffer.clear(); // cleaning buffer values                
                CS = START;
                break;

            case ASIGN:
  				buffer.push_back(c); // converting 1 char to string
  				this->addToTable(ASIGN, buffer, countLines, countPosition, dataFlag); 
  				buffer.clear(); // cleaning buffer values  
                CS = START;
                break;

            case MATH:
  				buffer.push_back(c); // converting 1 char to string
  				this->addToTable(MATH, buffer, countLines, countPosition, dataFlag); 
  				buffer.clear(); // cleaning buffer values  
                CS = START;
                break;

            case ENDSTR:
  				buffer.push_back(c); // converting 1 char to string
  				this->addToTable(ENDSTR, buffer, countLines, countPosition, dataFlag);
  				buffer.clear(); 	// cleaning buffer values   
                CS = START;
                break;

            case DELIMETR:
  				buffer.push_back(c); // converting 1 char to string
  				this->addToTable(DELIMETR, buffer, countLines, countPosition, dataFlag);
  				buffer.clear(); 	// cleaning buffer values   
                CS = START;
                break;

            case EXIT:
  				this->addToTable(EXIT, buffer, countLines, countPosition, dataFlag);
  				buffer.clear(); // cleaning buffer values
                CS = START;
                break;

            case COMMAND:
  				this->addToTable(COMMAND, buffer, countLines, countPosition, dataFlag);
  				buffer.clear(); // cleaning buffer values
                CS = START;
                break;

            case OPERATION:
  				this->addToTable(OPERATION, buffer, countLines, countPosition, dataFlag);
  				buffer.clear(); // cleaning buffer values
                CS = START;
                break;

            case IF:
  				this->addToTable(IF, buffer, countLines, countPosition, dataFlag);
  				buffer.clear(); // cleaning buffer values
                CS = START;
                break;

            case ENDIF:
  				this->addToTable(ENDIF, buffer, countLines, countPosition, dataFlag);
  				buffer.clear(); // cleaning buffer values
                CS = START;
                break;    

            case ELSE:
  				this->addToTable(ENDELSE, buffer, countLines, countPosition, dataFlag);
  				buffer.clear(); // cleaning buffer values
                CS = START;
                break;

            case ENDELSE:
  				this->addToTable(ENDELSE, buffer, countLines, countPosition, dataFlag);
  				buffer.clear(); // cleaning buffer values
                CS = START;
                break;

            case LOOP:
  				this->addToTable(LOOP, buffer, countLines, countPosition, dataFlag);
  				buffer.clear(); // cleaning buffer values
                CS = START;
                break;

            case ENDLOOP:
  				this->addToTable(ENDLOOP, buffer, countLines, countPosition, dataFlag);
  				buffer.clear(); // cleaning buffer values
                CS = START;
                break;

            case ERROR:
  				this->addToTable(ERROR, buffer, countLines, countPosition, dataFlag);
  				buffer.clear(); // cleaning buffer values
                CS = START;
                break;
  		}
  	}
}

string LexemManager::checkDataType(string buffer, string dataFlag)
{
	dataFlag = dataTypeTable_->getDataType(buffer, dataFlag);
	return dataFlag;
}

void LexemManager::addToTable(enum States state, string value, int line,\
                                                  int position, string type) 
{
	lexemTable_->addToTable(state, value, line, position, type);
}

void LexemManager::printTable() 
{
	lexemTable_->printTable();
}

void LexemManager::doSyntaxRoutine()
{
    syntaxManager_->doSyntaxRoutine(lexemTable_);
}
#include "header.h"

using namespace std;

// Class LexemTable
LexemTable::LexemTable() {};

LexemTable::~LexemTable() 
{
	// Clear the whole table
	lexemTable_.clear();
}

void LexemTable::addToTable(enum States state, string value, int line, int position, string type) 
{
	if (state == VARIABLE) {
		LexemNode newNode(state, value, line, position, type);
		lexemTable_.push_back(newNode);
	} else {
		LexemNode newNode(state, value, line, position);
		lexemTable_.push_back(newNode);
	}
}

vector<LexemNode> LexemTable::getTable()
{
	return lexemTable_;
}

void LexemTable::printTable() 
{
	cout << "LEXEM TYPE |LINE POSTION|VALUE" << endl;
	
	for (auto i = lexemTable_.begin(); i != lexemTable_.end(); ++i) {

		switch(i->getState()) {
    		case DATATYPE:
    			cout << COLOR_CYN << "DATATYPE" << COLOR_OFF;
		    	cout << "    [" << i->getLine() << ",";
		    	cout << i->getPosition() << "]\t: " << i->getValue() << endl;
    			break;

    		case DELIMETR:
    			cout << COLOR_WHT << "DELIMETR" << COLOR_OFF;
		    	cout << "    [" << i->getLine() << ",";
		    	cout << i->getPosition() << "]\t: " << i->getValue() << endl;
    			break;

    		case ENDSTR:
    			cout << COLOR_WHT << "ENDSTR" << COLOR_OFF;
    			cout << "      [" << i->getLine() << ",";
    			cout << i->getPosition() << "]\t: " << i->getValue() << endl;
    			break;

    		case VARIABLE:
    			cout << COLOR_GRN << "VARIABLE" << COLOR_OFF;
    			cout << "    [" << i->getLine() << ",";
                cout << i->getPosition() << "]\t: " << i->getValue();
				cout << "\t->\t" << i->getType() << endl;
                break;

    		case EXIT:
    			cout << COLOR_YEL << "EXIT" << COLOR_OFF;
		    	cout << "        [" << i->getLine() << ",";
		    	cout << i->getPosition() << "]\t: " << i->getValue() << endl;
    			break;

    		case COMMAND:
    			cout << COLOR_YEL << "COMMAND" << COLOR_OFF;
		    	cout << "     [" << i->getLine() << ",";
    			cout << i->getPosition() << "]\t: " << i->getValue() << endl;
    			break;

		    case OPERATION:
		    	cout << COLOR_BLU << "OPERATION" << COLOR_OFF;
		    	cout << "   [" << i->getLine() << ",";
		    	cout << i->getPosition() << "]\t: " << i->getValue() << endl;
		    	break;

    		case MATH:
    			cout << "MATHEMATICS [" << i->getLine() << ",";
    			cout << i->getPosition() << "]\t: " << i->getValue() << endl;
    			break;

    		case ASIGN:
    			cout << COLOR_WHT << "ASIGN" << COLOR_OFF;
    			cout << "       [" << i->getLine() << ",";
    			cout << i->getPosition() << "]\t: " << i->getValue() << endl;
    			break;

			case IF:
				cout << COLOR_MAG << "IF" << COLOR_OFF;
				cout << "          [" << i->getLine() << ",";
    			cout << i->getPosition() << "]\t: " << i->getValue() << endl;
    			break;

		    case ENDIF:
		    	cout << COLOR_MAG << "ENDIF" << COLOR_OFF;
		    	cout << "       [" << i->getLine() << ",";
		    	cout << i->getPosition() << "]\t: " << i->getValue() << endl;
            	break;

		    case ELSE:
		    	cout << COLOR_MAG << "ELSE" << COLOR_OFF;
		    	cout << "        [" << i->getLine() << ",";
		    	cout << i->getPosition() << "]\t: " << i->getValue() << endl;
	            break;

	        case ENDELSE:
	        	cout << COLOR_MAG << "ENDELSE" << COLOR_OFF;
	        	cout << "     [" << i->getLine() << ",";
	        	cout << i->getPosition() << "]\t " << i->getValue() << endl;
		        break;

		    case LOOP:
		    	cout << COLOR_MAG << "LOOP" << COLOR_OFF;
		    	cout << "        [" << i->getLine() << ",";
		    	cout << i->getPosition() << "]\t: " << i->getValue() << endl;
		    	break;

		    case ENDLOOP:
		    	cout << COLOR_MAG << "ENDLOOP" << COLOR_OFF;
		    	cout << "     [" << i->getLine() << ",";
		    	cout << i->getPosition() << "]\t: " << i->getValue() << endl;
		        break;

		    case ERROR:
		    	cout << COLOR_RED << "LEXEM ERROR" << COLOR_OFF;
		    	cout << " [" << i->getLine() << ",";
		    	cout << i->getPosition() << "]\t: " << i->getValue() << endl;
		    	break;

		    default:
		    	break;	
    	}
	}
}
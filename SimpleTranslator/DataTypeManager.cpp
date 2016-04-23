#include "header.h"

using namespace std;

// Class Data type manager
DataTypeManager::DataTypeManager() {};

DataTypeManager::~DataTypeManager() {};

string DataTypeManager::getDataType(string variable, string type)
{
	// identified pair
  	pair<map<string, string>::iterator, bool> result;

	result = dataTypeMap_.insert(pair<string, string>(variable, type));

	if (result.second == false) {
		//cout << "variable " << variable << " already existed" << endl;
		type = result.first->second;
	}
	
	return type;
}
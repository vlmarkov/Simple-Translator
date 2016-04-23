#include "header.h"

using namespace std;

// Class LexemNode
LexemNode::LexemNode(enum States newState, string newValue, int newLine, int newPositon) 
{
	// Fill private fields
	lexemNodeState_ 	= newState;
	lexemNodeValue_ 	= newValue;
	lexemNodeLine_ 		= newLine;
	lexemNodePosition_ 	= newPositon;
}

LexemNode::LexemNode(enum States newState, string newValue, int newLine, int newPositon, string newType) 
{
	// Fill private fields
	lexemNodeState_ 	= newState;
	lexemNodeValue_ 	= newValue;
	lexemNodeLine_ 		= newLine;
	lexemNodePosition_ 	= newPositon;
	lexemNodeDataType_  = newType;
}

LexemNode::~LexemNode() {}

int LexemNode::getLine() 
{ 
	return lexemNodeLine_; 
}

int LexemNode::getPosition() 
{ 
	return lexemNodePosition_; 
}

enum States LexemNode::getState() 
{ 
	return lexemNodeState_; 
}

string LexemNode::getType() 
{ 
	return lexemNodeDataType_; 
}

string LexemNode::getValue() 
{ 
	return lexemNodeValue_; 
}
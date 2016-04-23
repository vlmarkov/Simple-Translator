#include "header.h"

/* 
 * Syntax matrix of the relationship 
 * discrebed in linked list,
 * because matrix discharged
 */
SyntaxNode::SyntaxNode(int i, int j, int s): i_(i), j_(j), s_(s) {}
SyntaxNode::~SyntaxNode() {}

int SyntaxNode::getI() { return i_; }
int SyntaxNode::getJ() { return j_; }
int SyntaxNode::getS() { return s_; }
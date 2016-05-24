#ifndef _SYNTAX_ANALYZER_
#define _SYNTAX_ANALYZER_

#include "grammar.h"
#include "LLTable.h"

struct Node {
	// the derivation this node correspond
	int type;
	// the position to match
	int matchCursor;
	// father node
	Node* parent;
	// store the sons pointer correspond to
	// each mark in the right value of derivation
	std::vector<Node*> next;

	// current left position
	int printCursor;
	// top position
	int top;
	// font properties
	int fontSize, fontStyle;
	// not NULL if the node is a token
	std::string* lexname;

	Node(int type, Node* parent) : type(type), parent(parent) {
		matchCursor = 1;
		fontSize = 0;
		lexname = NULL;
	}
	void SetPosition(int newCursor, int newTop) {
		printCursor = newCursor;
		top = newTop;
	}
};

class SyntaxAnalyzer{
private:
	Grammar grammar;
	LLTable table;
	Node* root;
	Node* mCurrentNode;
public:
	void Init(const char* grammarInput, const char* mapInput, const char* LLTableInput);
	int MatchToken(int token, std::string& lexname);
	void AssignLexname(std::string lexname);
	void DetermineDepth(Node* node);
	void DetermineDepth();
	int PrintOneScript(int pos, Node* currentNode, int nextNumber);
	int PrintAllScript(Node* currentNode);
	int PrintSentence(Node* currentNode);
	void Print();
};

#endif
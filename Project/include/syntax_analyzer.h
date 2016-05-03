#ifndef _SYNTAX_ANALYZER_
#define _SYNTAX_ANALYZER_

struct Grammar {
	// the number of the derivation
	int derivationNumber;
	// store the derivation sign by sign
	// the first sign is the left value
	std::vector< std::vector<int> > derivation;
	// used for restore the derivation from token string
	std::map<int , std::string> translate;
	// debug output stream
	std::ofstream debugOutStream;

	// read in the derivation from grammarInput
	// read in the token-lex map
	void Init(const char* grammarInput, const char* mapInput);
	// return the i-th sign in k-th derivation
	int FetchSign(int k, int i);
	// print all the derivations for debug
	void Print(int k);
};

struct LLTable {
	// given mark-token pair and return action
	std::map<std::pair<int , int>, int> table;

	// build the table from LLTableInput
	// LLTableInput contains several line of the format:
	// mark token action
	void Init(const char* LLTableInput);
	// given mark-token pair and return action
	int Derivate(int mark, int token);
};

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
	int MatchToken(int token);
	void AssignLexname(std::string lexname);
	void DetermineDepth(Node* node);
	void DetermineDepth();
	int PrintOneScript(int pos, Node* currentNode, int nextNumber);
	int PrintAllScript(Node* currentNode);
	int PrintSentence(Node* currentNode);
	void Print();
};

#endif
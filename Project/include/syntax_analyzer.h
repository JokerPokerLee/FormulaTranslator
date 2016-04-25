#ifndef _SYNTAX_ANALYZER_
#define _SYNTAX_ANALYZER_

struct Grammar {

	int derivationNumber;
	std::vector< std::vector<int> > derivation;
	std::map<int , std::string> translate;

	void Init(const char* grammarInput, const char* mapInput);
	int FetchSign(int k, int i);
	void Print(int k);
};

struct LLTable {

	std::map<std::pair<int , int>, int> table;

	void Init(const char* LLTableInput);
	int Derivate(int mark, int token);
};

struct Node {

	int type;
	Node* parent;
	std::vector<Node*> next;

	int cursor;
	int fontStyle;
	int fontSize, top;
	std::string* lexname;

	Node(int type, Node* parent) : type(type), parent(parent) {
		fontSize = 0;
		lexname = NULL;
	}
	void SetPosition(int newCursor, int newTop) {
		cursor = newCursor;
		top = newTop;
	}
};

class SyntaxAnalyzer{
private:
	Grammar grammar;
	LLTable table;
	Node* root;
	Node* currentNode;
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
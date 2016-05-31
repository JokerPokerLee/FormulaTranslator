#include <bits/stdc++.h>

#define MINIMUM_SIZE	5
#define MINIMUM_ROOT	40

//[0000, 0100) succ type
#define SUCC			0000
#define DRVT_COMPLETE	0001

//[0100, 1000) error type
#define EXCEED_DRVT		0100
#define NO_LL_RULE		0101
#define MISMATCH_TOKEN	0102
#define INVALID_TOKEN	0103
#define END_OF_FILE		0104
#define DRVT_TERMINATED	0105

//[1000, 2000) token name
#define INTEGRITY		1000
#define SIGMA			1001
#define BLANK			1002
#define SUBSCRIPT		1003
#define ALLSCRIPT		1004
#define SUPERSCRIPT		1005
#define LEFTCURLY		1006
#define RIGHTCURLY		1007
#define LEFTBRACKET		1008
#define RIGHTBRACKET	1009
#define NUMBER			1010
#define ID				1011
#define DOLLAR			1012
#define EMPTY			1013

//[2000, 3000) mark name
#define SMARK			2000
#define FMARK			2001
#define BMARK			2002

// #define FONT_FAMILY "Anonymous"
#define FONT_FAMILY "Monaco"
// #define FONT_FAMILY "monospace"

class HtmlPrinter {
private:
	static FILE* htmlOutputFile;
public:
	static void Init(const char* formulaOutput);
	//fontStyle = 0 when wants normal
	static int PrintToken(int left, int top, int fontSize, int fontStyle, std::string& token, int len);
	static void Finish();
};

FILE* HtmlPrinter::htmlOutputFile;

void HtmlPrinter::Init(const char* formulaOutput) {
	htmlOutputFile = fopen(formulaOutput, "w");
	fprintf(htmlOutputFile, "<html>\n");
	fprintf(htmlOutputFile, "\t<head>\n");
	fprintf(htmlOutputFile, "\t\t<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\" />\n");
	fprintf(htmlOutputFile, "\t</head>\n\t<body>\n");
}

int HtmlPrinter::PrintToken(int left, int top, int fontSize, int fontStyle, std::string& token, int len) {
	// continuous ID need to be separated by space
	// a token with leading space mark the situation above
	if (token[0] == ' ') {
		left += fontSize * 2 / 5;
		token = token.substr(1, token.size() - 1);
		len--;
	}
	fprintf(htmlOutputFile, "\t\t<div style=\"position: absolute; left:%dpx; top:%dpx;\">\n", left, top);
	fprintf(htmlOutputFile, "\t\t\t<span style=\"font-family:%s; font-size:%dpx; font-style:%s; line-height:100%%;\">\n", FONT_FAMILY, fontSize, fontStyle ? "oblique" : "normal");
	fprintf(htmlOutputFile, "\t\t\t\t%s\n\t\t\t</span>\n\t\t</div>\n", token.c_str());
	return left + len * fontSize * 3 / 5;
}

void HtmlPrinter::Finish() {
	fprintf(htmlOutputFile, "\t</body>\n</html>\n");
	fclose(htmlOutputFile);
}

class ErrorReporter {
private:
	static std::string* formula;
public:
	static void Init(std::string* formula_ptr) {
		formula = formula_ptr;
	};
	static void PrintPosition(int tokenLength);
};

std::string* ErrorReporter::formula;

void ErrorReporter::PrintPosition(int tokenLength) {
	std::cout << "\t" << *formula << "\n\t";
	for (unsigned int i = 0; i < (*formula).size() - tokenLength; i++)
		std::cout << " ";
	std::cout << "^\n\n";
}

struct TokenDFA {
	int totalState;
	int startSta;
	int currentSta;
	std::map<int , int> foundSta;
	std::vector< std::map<char , int> > link;

	void Init();
	void InsertRule(int state1, int state2, char ch);
	// walk along the dfa.
	// if there is no path state remain still
	// return SUCC if path exist, INVALID_TOKEN otherwise
	int Next(char ch);
	bool FoundToken();
	void Print();
};

void TokenDFA::Init() {
	totalState = 0;
	startSta = 0;
	foundSta.clear();
	link.clear();
}

void TokenDFA::InsertRule(int state1, int state2, char ch) {
	totalState = std::max(totalState, std::max(state1 + 1, state2 + 1));
	link.resize(totalState);
	if (link[state1].count(ch)) {
		std::cout << "Collisional rules in TokenDFA." << std::endl;
		exit(-1);
	}
	link[state1][ch] = state2;
}

int TokenDFA::Next(char ch) {
	if (link[currentSta].count(ch)) {
		currentSta = link[currentSta][ch];
		return SUCC;
	}
	return INVALID_TOKEN;
}

bool TokenDFA::FoundToken() {
	return foundSta.count(currentSta);
}

void TokenDFA::Print() {
	for (int i = 0; i < totalState; i++)
		for (int j = 0; j < 256; j++) {
			if (link[i].count((char)j))
				std::cout << i << " " << link[i][(char)j] << " " << (char)j << std::endl;
		}
}

class LexicalAnalyzer {
private:
	// DFA rule file
	FILE* ruleInput;
	// formula input file
	FILE* formulaInput;
	// DFA
	TokenDFA autoMachine;
	// input formula string
	std::string formula;
	// print token to debug file
	std::ofstream debugOutStream;
public:
	// initialize lexical analyzer
	void Init(const char* ruleInput, char* formulaInput);
	// build DFA from DFA rule file
	void BuildTokenDFA(std::string fileName);
	// start from the start state in DFA
	// walk along the DFA till the finite state
	int GetNextToken(int& token, std::string& lexname);
	// print DFA information for debug
	void PrintTokenDFA();
	// bind formula string to error reporter
	std::string* FormulaPointer();
};

void LexicalAnalyzer::Init(const char* ruleInput, char* formulaInputFile) {
	autoMachine.Init();
	formula = "";
	BuildTokenDFA(std::string(ruleInput));
	formulaInput = fopen(formulaInputFile, "r");
	debugOutStream.open("../output/token.out");
}

void LexicalAnalyzer::BuildTokenDFA(std::string fileName) {
	ruleInput = fopen(fileName.c_str(), "r");
	for (int i = 0; i < 13; i++) {
		int state;
		fscanf(ruleInput, "%d", &state);
		autoMachine.foundSta[state] = 1000 + i;
	}
	fscanf(ruleInput, "%d", &autoMachine.startSta);
	int state1, state2;
	char ch;
	while (fscanf(ruleInput, "%d %d%*c%c%*c", &state1, &state2, &ch) != EOF) {
		if (ch == 'A') {
			for (int i = 0; i < 26; i++) {
				autoMachine.InsertRule(state1, state2, char('a' + i));
				autoMachine.InsertRule(state1, state2, char('A' + i));
			}
		} else
		if (ch == '0') {
			for (int i = 0; i < 10; i++) {
				autoMachine.InsertRule(state1, state2, char('0' + i));
			}
		} else
		if (ch == 'S') {
			autoMachine.InsertRule(state1, state2, ' ');
		} else
		if (ch == 'E') {
			autoMachine.InsertRule(state1, state2, '\n');
		} else
		autoMachine.InsertRule(state1, state2, ch);
	}
	fclose(ruleInput);
}

int LexicalAnalyzer::GetNextToken(int &token, std::string &lexname) {
	// every time start from the origin state
	autoMachine.currentSta = autoMachine.startSta;

	// keep trying to walk along the dfa until no
	// edge corresponding to the read in charactor
	// if reach the end of file, then break
	char ch;
	std::string currentToken = "";
	while (autoMachine.Next(ch = (char)getc(formulaInput)) != INVALID_TOKEN) {
		if (ch != ' ' && ch != '\n') {
			currentToken += ch;
		}
		formula += ch;
	}

	if (ch != EOF) {
		fseek(formulaInput, -1, SEEK_CUR);
	}

	// if no characters read in, then it reach the end of the file
	if (ch == EOF && currentToken.empty()) {
		return END_OF_FILE;
	}

	if (autoMachine.FoundToken()) {		// found a token
		// formula += currentToken;
		token = autoMachine.foundSta[autoMachine.currentSta];
		lexname = currentToken;
		// record for debug
		debugOutStream << token << " : " << lexname << std::endl;
		return SUCC;
	} else {							// invalid char OR!!! EOF
		while (!feof(formulaInput)) {
			char ch = (char)getc(formulaInput);
			if (autoMachine.link[autoMachine.startSta].count(ch)) {
				fseek(formulaInput, -1, SEEK_CUR);
				break;
			}
			currentToken += ch;
		}
		formula += currentToken;
		std::cout << "lexical_analyzer: error" << ": ";
		std::cout << "invalid token \"" << currentToken << "\" detected." << std::endl;
		ErrorReporter::PrintPosition(currentToken.size());
		return INVALID_TOKEN;
	}
}

void LexicalAnalyzer::PrintTokenDFA() {
	autoMachine.Print();
}

std::string* LexicalAnalyzer::FormulaPointer() {
	return &formula;
}

struct Grammar {
	// the number of the derivation
	int derivationNumber;
	// store the derivation sign by sign
	// the first sign is the left value
	std::vector< std::vector<int> > derivation;
	// used for restore the derivation from token string
	// return the lexname of given token number
	std::map<int , std::string> translate;
	// debug output stream
	std::ofstream debugOutStream;

	// read in the derivation from grammarInput
	// read in the token-lex map
	void Init(const char* grammarInput, const char* mapInput);
	// return the i-th sign in k-th derivation
	int FetchSign(int k, int i);
	// print k-th derivations
	void Print(int k);
};

void Grammar::Init(const char* grammarInput, const char* mapInput) {
	derivationNumber = 0;
	derivation.clear();
	FILE* fp = fopen(grammarInput, "r");
	while (true) {
		std::string currentSentence = "";
		char ch;
		// use the \n and EOF to seperate each derivation
		while ((ch = fgetc(fp)) != '\n' && ch != EOF)
			currentSentence += ch;
		if (currentSentence.size() == 0) break;
		derivationNumber++;
		derivation.resize(derivationNumber);
		for (unsigned int i = 0; i < currentSentence.size(); i++) {
			int code;
			// rematch the signs with single char
			switch (currentSentence[i]) {
				case 'S': code = SMARK;			break;
				case 'B': code = BMARK;			break;
				case 'F': code = FMARK;			break;
				case '$': code = DOLLAR;		break;
				case '&': code = ALLSCRIPT;		break;
				case '^': code = SUPERSCRIPT;	break;
				case '_': code = SUBSCRIPT;		break;
				case 'i': code = INTEGRITY;		break;
				case 's': code = SIGMA;			break;
				case 'd': code = ID;			break;
				case 'n': code = NUMBER;		break;
				case 'b': code = BLANK;			break;
				case '(': code = LEFTBRACKET;	break;
				case ')': code = RIGHTBRACKET;	break;
				case '{': code = LEFTCURLY;		break;
				case '}': code = RIGHTCURLY;	break;
				case 'e': code = EMPTY;			break;
			}
			derivation.back().push_back(code);
		}
	}
	fclose(fp);

	// to generate readable debug for grammar init
	fp = fopen(mapInput, "r");
	int sign;
	char* lexname = new char[10];
	while (fscanf(fp, "%d %s", &sign, lexname) != EOF) {
		translate[sign] = std::string(lexname);
	}
	delete lexname;
	fclose(fp);

	// output all the derivation uesd in process
	debugOutStream.open("../output/derivation.out");
	debugOutStream << "Left most derivation:" << std::endl;
}

int Grammar::FetchSign(int k, int i) {
	// no such sentence or exceed the sentence length
	if (k >= derivationNumber || (unsigned int)i >= derivation[k].size()) {
		return EXCEED_DRVT;
	}
	// F -> emplty
	if (derivation[k][i] == EMPTY) return EXCEED_DRVT;
	return derivation[k][i];
}

void Grammar::Print(int k) {
	debugOutStream << translate[derivation[k][0]] << " -> ";
	for (unsigned int i = 1; i < derivation[k].size(); i++) {
		debugOutStream << translate[derivation[k][i]];
	}
	debugOutStream << std::endl;
}

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

void LLTable::Init(const char* LLTableInput) {
	// read in LL table
	FILE* fp = fopen(LLTableInput, "r");
	int mark, token, derivation;
	while (fscanf(fp, "%d %d %d", &mark, &token, &derivation) != EOF) {
		std::pair<int , int> mtp = std::make_pair(mark, token);
		if (table.count(mtp) && table[mtp] != derivation) {
			std::cout << "Collision rules in LL table." << std::endl;
			return;
		}
		table[mtp] = derivation;
	}
	fclose(fp);
}

// query LL table term
int LLTable::Derivate(int mark, int token) {
	std::pair<int , int> mtp = std::make_pair(mark, token);
	if (table.count(mtp)) {
		return table[mtp];
	}
	return NO_LL_RULE;
}

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

	Node(int t, Node* p) : type(t), parent(p) {
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
	// record if last token printed is ID
	bool isLastID;
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

static std::string leftCurly = std::string("(");
static std::string rightCurly = std::string(")");

void SyntaxAnalyzer::Init(const char* grammarInput, const char* mapInput, const char* LLTableInput) {
	grammar.Init(grammarInput, mapInput);
	table.Init(LLTableInput);
	// first derivation
	grammar.Print(1);
	mCurrentNode = root = new Node(1, NULL);
	isLastID = false;
}

// read in token and decide to derivate or move on
int SyntaxAnalyzer::MatchToken(int token, std::string& lexname) {
	bool errorDetected = false;
	while (true) {
		// sign stands for the expect sign in LL derivation
		int sign = grammar.FetchSign(mCurrentNode -> type, mCurrentNode -> matchCursor);
		// matching process in current sentence is done
		if (sign == EXCEED_DRVT) {
			mCurrentNode = mCurrentNode -> parent;
			if (mCurrentNode != NULL) {
				mCurrentNode -> matchCursor++;
				continue;
			} else {
				return DRVT_TERMINATED;
			}
		}
		// if the expect sign is a terminal token
		// match the token
		if (sign < 2000) {
			if (token == sign) {
				// if match, push cursor
				mCurrentNode -> matchCursor++;
				if (mCurrentNode -> type == 1 && mCurrentNode -> matchCursor == 5)
					return DRVT_COMPLETE;
				if (token == ID || token == NUMBER || token == BLANK)
					AssignLexname(token != BLANK ? lexname : std::string(" "));
				// consider the error detected before
				return errorDetected ? MISMATCH_TOKEN : SUCC;
			} else {
				// print error info
				std::cout << "syntax_analyzer: error: ";
				std::cout << "expect \'" << grammar.translate[sign] << "\' before \'" << lexname << "\' token.\n";
			}
		}
		// if the expect sign is a non-terminal token
		// derivate recursively
		if (sign >= 2000) {
			int derivation = table.Derivate(sign, token);
			if (derivation != NO_LL_RULE) {
				// find a rule and derivate
				grammar.Print(derivation);		// used for debug
				mCurrentNode -> next.push_back(new Node(derivation, mCurrentNode));
				mCurrentNode = mCurrentNode -> next.back();
				continue;
			} else {
				// no match rule, print error info
				std::string info;
				std::cout << "syntax_analyzer: error: ";
				switch (sign) {
					case SMARK:
						info = std::string("expect \'$\' as a start symbol.");
						break;
					case FMARK:
						info = std::string("expect transcript type specification.");
						break;
					case BMARK:
						info = std::string("expect a formula before \'");
						info += grammar.translate[token];
						info += std::string("\' token.");
						break;
				}
				std::cout << info << std::endl;
			}
		}
		errorDetected = true;
		// token mismatched, print error position
		ErrorReporter::PrintPosition(lexname.size());
		// Errors Toleration
		// if token is a kind of brackets then discard the token
		if (token >= LEFTCURLY && token <= RIGHTBRACKET)
			break;
		// otherwise, discard current node
		// because there's always an F following
		// check if sign = dollar, if not, the first symbol error
		// may cause program end immediately
		if (sign == DOLLAR) {
			mCurrentNode -> matchCursor++;
		} else {
			mCurrentNode = mCurrentNode -> parent;
		}
	}
	return errorDetected ? MISMATCH_TOKEN : SUCC;
}

// only leaf node can have lexname
void SyntaxAnalyzer::AssignLexname(std::string lexname) {
	// lexname is a pointer to string
	// must assign a new storage area
	mCurrentNode -> lexname = new std::string(lexname);
}

// recursive determine minimum size
void SyntaxAnalyzer::DetermineDepth(Node* node) {
	// default font size
	node -> fontSize = MINIMUM_SIZE;
	for (unsigned int i = 0; i < node -> next.size(); i++) {
		DetermineDepth(node -> next[i]);
		// indicate whether to shrink
		int ratio = 0;
		// have only supscript or subscript
		if (node -> type >= 5 && node -> type <= 6)
			ratio = i < 2;
		// have both supscript and subscript
		if (node -> type == 4 || (node -> type >= 7 && node -> type <= 8))
			ratio = i < 4;
		node -> fontSize = std::max(node -> fontSize, node -> next[i] -> fontSize * (ratio + 1));
	}
}

// first find the minimum
// second adjust the minimum
// third update descendant size from root use bfs
void SyntaxAnalyzer::DetermineDepth() {
	// calc least need font size of root
	DetermineDepth(root);
	// minimum root node font size;
	root -> fontSize = std::max(root -> fontSize, MINIMUM_ROOT);
	// use bfs method to update descendant font size
	std::queue<Node*> nodeQueue;
	nodeQueue.push(root);
	while (nodeQueue.size()) {
		Node* now = nodeQueue.front();
		nodeQueue.pop();
		for (unsigned int i = 0; i < now -> next.size(); i++) {
			// indicate whether to shrink
			int ratio = 0;
			// have only supscript or subscript
			if (now -> type >= 5 && now -> type <= 6)
				ratio = i < 2;
			// have both supscript and subscript
			if (now -> type == 4 || (now -> type >= 7 && now -> type <= 8))
				ratio = i < 4;
			now -> next[i] -> fontSize = now -> fontSize / (ratio + 1);
			nodeQueue.push(now -> next[i]);
		}
	}

	//check result
}

// current node has only subscript or supscript
// pos indicate the sub(1) or the sup(0)
// print only script
int SyntaxAnalyzer::PrintOneScript(int pos, Node* currentNode, int nextNumber) {
	//current info tmp
	int currentTop = currentNode -> top;
	int currentCursor = currentNode -> printCursor;
	int currentSize = currentNode -> fontSize;

	//next info tmp
	int nextSize = currentSize / 2;
	Node* nextNode = currentNode -> next[nextNumber];
	// pos = 0 when solve superscript vise verse subscript pos = 1
	int nextTop = currentTop + pos * (currentSize - nextSize);
	nextTop += pos * (nextSize / 5);
	nextNode -> SetPosition(currentCursor, nextTop);
	return PrintSentence(nextNode);
}

// print scripts and others
int SyntaxAnalyzer::PrintAllScript(Node* currentNode) {
	int currentTop = currentNode -> top;
	int originCursor = currentNode -> printCursor;
	int& currentCursor = currentNode -> printCursor;
	// ID in different scripts has no need to be sperated
	isLastID = false;
	//print subscript
	currentCursor = originCursor;
	currentCursor = PrintOneScript(1, currentNode, 0);
	currentCursor = PrintOneScript(1, currentNode, 1);
	// ID in different scripts has no need to be sperated
	isLastID = false;
	//print superscript
	currentCursor = originCursor;
	currentCursor = PrintOneScript(0, currentNode, 2);
	currentCursor = PrintOneScript(0, currentNode, 3);
	// ID in different scripts has no need to be sperated
	isLastID = false;
	//push cursor and print suffix
	currentCursor = std::max(currentNode -> next[1] -> printCursor, currentNode -> next[3] -> printCursor);
	currentNode -> next[4] -> SetPosition(currentCursor, currentTop);
	currentCursor = PrintSentence(currentNode -> next[4]);
	if (currentNode -> type >= 7) {
		currentNode -> next[5] -> SetPosition(currentCursor, currentTop);
		currentCursor = PrintSentence(currentNode -> next[5]);
	}
	return currentCursor;
}

// print all kinds of formulas
int SyntaxAnalyzer::PrintSentence(Node* currentNode) {
	// current node info tmp
	int currentTop = currentNode -> top;
	int currentType = currentNode -> type;
	int currentSize = currentNode -> fontSize;
	int& currentCursor = currentNode -> printCursor;
	// F -> empty
	if (currentType == 3) return currentCursor;
	// terminal tokens directly print
	if (currentType >= 9 && currentType <= 11) {
		std::string token = *(currentNode -> lexname);
		int style = currentType == 9 ? 1 : 0;
		if ((currentType == 9 || currentType == 10) && isLastID) {
			// continuous ID need to be separated by space
			token = std::string(" ") + token;
		}
		currentCursor = HtmlPrinter::PrintToken(currentCursor, currentTop, currentSize, style, token, token.size());
		isLastID = currentType == 9;
		return currentNode -> printCursor;
	}
	int pos, delta;
	std::string token;
	switch (currentNode -> type) {
		case 1:
		case 2:
			for (unsigned int i = 0; i < currentNode -> next.size(); i++) {
				currentNode -> next[i] -> SetPosition(currentCursor, currentTop);
				currentCursor = PrintSentence(currentNode -> next[i]);
			}
			break;
		case 4:
			currentCursor = PrintAllScript(currentNode);
			// if print script, then the separate spaces are no longer needed
			// same situations below
			break;
		case 5:
		case 6:
			pos = (currentNode -> type + 1) & 1;
			isLastID = false;
			currentCursor = PrintOneScript(pos, currentNode, 0);
			currentCursor = PrintOneScript(pos, currentNode, 1);
			currentNode -> next[2] -> SetPosition(currentCursor, currentTop);
			isLastID = false;
			currentCursor = PrintSentence(currentNode -> next[2]);
			break;
		case 7:
		case 8:
			if (currentNode -> type & 1) {
				token = "กา";
				delta = 5;
			} else {
				token = "ฆฒ";
				delta = 5;
			}
			currentCursor = HtmlPrinter::PrintToken(currentCursor, currentTop - delta * 4 / 5, currentSize + delta, 0, token, 1);
			isLastID = false;
			currentCursor = PrintAllScript(currentNode);
			break;
		case 12:
			currentCursor = HtmlPrinter::PrintToken(currentCursor, currentTop, currentSize, 0, leftCurly, 1);
			isLastID = false;
			for (unsigned int i = 0; i < currentNode -> next.size(); i++) {
				currentNode -> next[i] -> SetPosition(currentCursor, currentTop);
				currentCursor = PrintSentence(currentNode -> next[i]);
			}
			currentCursor = HtmlPrinter::PrintToken(currentCursor, currentTop, currentSize, 0, rightCurly, 1);
			isLastID = false;
			break;
		default:
			//ERROR
			break;
	}
	return currentNode -> printCursor;
}

// outer interface
void SyntaxAnalyzer::Print() {
	// initialize root position
	root -> printCursor = 100;
	root -> top = 300;
	PrintSentence(root);
}

int main(int argc, char* argv[]) {

	char* formulaInputFile;

	if (argc == 1) {
		std::cout << "Please input the formula code:" << std::endl;
		std::cout << "(input should be ended with #)" << std::endl;
		FILE* fp = fopen("../input/formula.in", "w");
		char ch;
		while ((ch = (char)getchar()) != '#') {
			if (ch != '\n') {
				fputc(ch, fp);
			}
		}
		fclose(fp);
		std::cout << "\n\n";
		formulaInputFile = (char*)"../input/formula.in";
	} else {
		formulaInputFile = argv[1];
	}

	std::cout << "Start initializing lexical analyzer." << std::endl;
	LexicalAnalyzer lexicalAnalyzer;
	lexicalAnalyzer.Init("../input/rule.in", formulaInputFile);
	std::cout << "Lexical analyzer initialization complete.\n" << std::endl;

	std::cout << "Start initializing syntax analyzer." << std::endl;
	SyntaxAnalyzer syntaxAnalyzer;
	syntaxAnalyzer.Init("../input/sentence.in", "../input/map.in", "../input/LLTable.in");
	std::cout << "Syntax analyzer initialization complete.\n" << std::endl;

	std::cout << "Start initializing html printer." << std::endl;
	HtmlPrinter::Init("../output/result.html");
	std::cout << "Html printer initialization complete.\n" << std::endl;

	std::cout << "The tokens read in will be recorded in \"output/token.out\"." << std::endl;
	std::cout << "The derivations used will be recorded in \"output/derivation.out\".\n" << std::endl;

	std::cout << "Bind formula string to error reporter." << std::endl;
	ErrorReporter::Init(lexicalAnalyzer.FormulaPointer());
	std::cout << "Formula string successfully binded to error reporter.\n" << std::endl;

	std::cout << "===================Start derivating.===================" << std::endl << std::endl;
	int token;
	std::string lexname;
	bool lexicalError = false;
	bool syntaxError = false;
	while (true) {
		// return value
		int rtn;

		// fetch another token
		rtn = lexicalAnalyzer.GetNextToken(token, lexname);
		// if cannot fetch another token
		// the derivation must be incomplete at that time
		// otherwise it would have breaked
		// so there's only one possibility that the formula is incomplete
		if (rtn == END_OF_FILE) {
			std::cout << "lexical_analyzer: error" << ": ";
			std::cout << "The formula should be ended with $.\n" << std::endl;
			lexicalError = true;
			break;
		}
		if (rtn == INVALID_TOKEN) {
			lexicalError = true;
			continue;
		}

		rtn = syntaxAnalyzer.MatchToken(token, lexname);
		if (rtn == DRVT_COMPLETE) {
			break;
		}
		if (rtn == DRVT_TERMINATED) {
			std::cout << "syntax_analyzer: error" << ": ";
			std::cout << "The formula should be ended with $.\n" << std::endl;
			syntaxError = true;
			break;
		}
		if (rtn == MISMATCH_TOKEN) {
			syntaxError = true;
		}
	}

	if (lexicalError || syntaxError) {
		std::cout << "==========Derivation terminated with error(s).=========\n" << std::endl;

        // show message, not quit immediately
        char quitCh;
        while ((quitCh = getchar()) != 'q' && quitCh != 'Q');

		return 0;
	} else {
		std::cout << "No error detected.\n" << std::endl;
	}

	std::cout << "=================Derivation completed.=================\n" << std::endl;

	std::cout << "Start calc the height of the grammar tree." << std::endl;
	syntaxAnalyzer.DetermineDepth();
	std::cout << "Height calculation complete.\n" << std::endl;

	std::cout << "Start printing formula." << std::endl;
	syntaxAnalyzer.Print();
	HtmlPrinter::Finish();
	std::cout << "The result has been written in \"output/result.html\".\n" << std::endl;

	std::cout << "Formula translation complete." << std::endl;

    std::cout << "Press q to quit." << std::endl;

    // show message, not quit immediately
    char quitCh;
    while ((quitCh = getchar()) != 'q' && quitCh != 'Q');

	return 0;
}

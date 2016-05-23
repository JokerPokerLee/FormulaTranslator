#ifndef _LEXICAL_ANALYZER_
#define _LEXICAL_ANALYZER_

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

class LexicalAnalyzer {
private:
	FILE* ruleInput;
	FILE* formulaInput;
	TokenDFA autoMachine;
	std::string formula;
	std::ofstream debugOutStream;
public:
	void Init(const char* ruleInput, const char* formulaInput);
	void BuildTokenDFA(std::string fileName);
	void InitFormulaInput(std::string fileName);
	int GetNextToken(int& token, std::string& lexname);
	void PrintTokenDFA();
	std::string* FormulaPointer();
};

#endif
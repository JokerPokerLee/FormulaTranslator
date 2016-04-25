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
	int Next(char ch);
	void Print();
};

class LexicalAnalyzer {
private:
	FILE* ruleInput;
	FILE* formulaInput;
	TokenDFA autoMachine;
	std::string formula;
public:
	void Init(const char* ruleInput, const char* formulaInput);
	void BuildTokenDFA(std::string fileName);
	void InitFormulaInput(std::string fileName);
	int GetNextToken(int& token, std::string& lexname);
	void PrintTokenDFA();
};

#endif
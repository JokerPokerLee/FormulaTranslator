#ifndef _LEXICAL_ANALYZER_
#define _LEXICAL_ANALYZER_

#include "tokenDFA.h"

class LexicalAnalyzer {
private:
	FILE* ruleInput;
	FILE* formulaInput;
	TokenDFA autoMachine;
	std::string formula;
	std::ofstream debugOutStream;
public:
	void Init(const char* ruleInput, char* formulaInput);
	void BuildTokenDFA(std::string fileName);
	int GetNextToken(int& token, std::string& lexname);
	void PrintTokenDFA();
	std::string* FormulaPointer();
};

#endif
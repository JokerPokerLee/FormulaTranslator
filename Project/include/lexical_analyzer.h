#ifndef _LEXICAL_ANALYZER_
#define _LEXICAL_ANALYZER_

#include "tokenDFA.h"

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

#endif
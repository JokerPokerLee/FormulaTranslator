#include <bits/stdc++.h>
#include "source.h"
#include "tokenDFA.h"
#include "error_reporter.h"
#include "lexical_analyzer.h"

void LexicalAnalyzer::Init(const char* ruleInput, const char* formulaInput) {
	autoMachine.Init();
	formula = "";
	BuildTokenDFA(std::string(ruleInput));
	InitFormulaInput(std::string(formulaInput));
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

void LexicalAnalyzer::InitFormulaInput(std::string fileName) {
	formulaInput = fopen(fileName.c_str(), "r");
}

int LexicalAnalyzer::GetNextToken(int &token, std::string &lexname) {
	// every time start from the origin state
	autoMachine.currentSta = autoMachine.startSta;

	// keep trying to walk along the dfa until no
	// edge corresponding to the read in charactor
	// if reach the end of file, then break
	std::string currentToken = "";
	while (!feof(formulaInput)) {
		char ch = (char)getc(formulaInput);
		if (autoMachine.Next(ch) == INVALID_TOKEN) {
			// the readin character cannot be eof and must
			// be an invalid char cause has checked in while
			// so can push back the cursor without check
			fseek(formulaInput, -1, SEEK_CUR);
			break;
		}
		if (ch != ' ' && ch != '\n') {
			currentToken += ch;
		}
	}

	// if no characters read in, then it reach the end of the file
	if (currentToken.empty()) {
		return END_OF_FILE;
	}

	if (autoMachine.FoundToken()) {		// found a token
		formula += currentToken;
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
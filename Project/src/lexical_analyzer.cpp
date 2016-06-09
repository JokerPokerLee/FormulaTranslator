#include <bits/stdc++.h>
#include "source.h"
#include "tokenDFA.h"
#include "error_reporter.h"
#include "lexical_analyzer.h"

int LexicalAnalyzer::Init(const char* ruleInput, char* formulaInputFile, std::string& logMesgFile) {
	autoMachine.Init();
	formula = "";
	BuildTokenDFA(std::string(ruleInput));
	formulaInput = fopen(formulaInputFile, "r");
	if (formulaInput == NULL) {
		std::cout << "Can't not open formula code file.\n" << std::endl;
		return NO_FORMULA_FILE;
	}
	logMesgFile = std::string("Project/log/token.out");
	debugOutStream.open("../log/token.out");
	if (debugOutStream.fail()) {
		std::cout << "***Can't not open Project/log/token.out file." << std::endl;
		std::cout << "***Simply output the token identified from input stream in the current directory." << std::endl;
		logMesgFile = std::string("./token.out");
		debugOutStream.open("./token.out");
		return LOG_FILE_FAIL;
	} else {
		return SUCC;
	}
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
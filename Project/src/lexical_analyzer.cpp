#include <bits/stdc++.h>
#include "source.h"
#include "error_reporter.h"
#include "lexical_analyzer.h"

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

	// keep trying to walk along the dfa
	// if reach the end of file, then break
	// if reach a sta contain valid token, then break
	std::string currentToken = "";
	while (!feof(formulaInput) && !autoMachine.FoundToken()) {
		char ch = (char)getc(formulaInput);
		if (ch != ' ' && ch != '\n') {
			currentToken += ch;
		}
		if (autoMachine.Next(ch) == INVALID_TOKEN) {
			break;
		}
		// special judge ALLSCRIPT
		if (!feof(formulaInput) && autoMachine.currentSta == 26) {
			ch = (char)getc(formulaInput);
			if (autoMachine.Next(ch) == INVALID_TOKEN) {
				fseek(formulaInput, -1, SEEK_CUR);
				break;
			}
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
		ErrorReporter::Error(formula, currentToken);
		return INVALID_TOKEN;
	}
}

void LexicalAnalyzer::PrintTokenDFA() {
	autoMachine.Print();
}
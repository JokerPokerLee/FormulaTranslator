#include <bits/stdc++.h>
#include "source.h"
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
		return 0;
	}
	return -1;
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
	for (int i = 0; i < 13; i++) {
		autoMachine.foundSta[i] = 1000 + i;
	}
	ruleInput = fopen(fileName.c_str(), "r");
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
	autoMachine.currentSta = autoMachine.startSta;
	std::string currentToken = "";
	bool error = false;
	while (!autoMachine.foundSta.count(autoMachine.currentSta)) {
		char ch;
		ch = (char)getc(formulaInput);
		if (currentToken.size() || ch != ' ' && ch != '\n') {
			currentToken += ch;
		}
		if (autoMachine.Next(ch)) {
			error = true;
			break;
		}
	}
	if (error) {
		std::cout << "Invalid string." << std::endl;
		return -1;
	}
	if (autoMachine.currentSta < 6 || autoMachine.currentSta == 10 || autoMachine.currentSta == 11) {
		fseek(formulaInput, -1, SEEK_CUR);
		currentToken.pop_back();
	}
	formula += currentToken;
	token = autoMachine.foundSta[autoMachine.currentSta];
	lexname = currentToken;
	debugOutStream << token << " : " << lexname << std::endl;
	return 0;
}

void LexicalAnalyzer::PrintTokenDFA() {
	autoMachine.Print();
}
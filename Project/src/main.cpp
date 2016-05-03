#include <bits/stdc++.h>
#include "source.h"
#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include "html_printer.h"

int main(int argc, char* argv[]) {
	SyntaxAnalyzer syntaxAnalyzer;
	LexicalAnalyzer lexicalAnalyzer;
	HtmlPrinter::Init("../output/result.html");
	lexicalAnalyzer.Init("../input/rule.in", "../input/formula.in");
	syntaxAnalyzer.Init("../input/sentence.in", "../input/map.in", "../input/LLTable.in");
	int token;
	std::string lexname;
	int cnt = 0;
	while (cnt < 2 && !lexicalAnalyzer.GetNextToken(token, lexname)) {
		int rtn;
		rtn = syntaxAnalyzer.MatchToken(token);
		// if (token == ID || token == NUMBER || token == BLANK)
		// 	syntaxAnalyzer.AssignLexname(token == BLANK ? lexname : std::string(" "));
		cnt += token == DOLLAR;
	}
	// syntaxAnalyzer.DetermineDepth();
	// syntaxAnalyzer.Print();
	return 0;
}
#include <bits/stdc++.h>
#include "source.h"
#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include "html_printer.h"

int main(int argc, char* argv[]) {
	LexicalAnalyzer lexicalAnalyzer;
	SyntaxAnalyzer syntaxAnalyzer;
	lexicalAnalyzer.Init("../data/rule.in", "../data/formula.in");
	syntaxAnalyzer.Init("../data/sentence.in", "../data/map.in", "../data/LLTable.in");
	int token;
	std::string lexname;
	int cnt = 0;
	HtmlPrinter::Init("../data/result.html");
	while (cnt < 2 && !lexicalAnalyzer.GetNextToken(token, lexname)) {
		int rtn;
		rtn = syntaxAnalyzer.MatchToken(token);
		if (token == ID || token == NUMBER || token == BLANK)
			syntaxAnalyzer.AssignLexname(token == BLANK ? lexname : std::string(" "));
		cnt += token == DOLLAR;
	}
	syntaxAnalyzer.DetermineDepth();
	syntaxAnalyzer.Print();
	return 0;
}
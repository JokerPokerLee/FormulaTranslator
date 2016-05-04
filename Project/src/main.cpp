#include <bits/stdc++.h>
#include "source.h"
#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include "html_printer.h"

int main(int argc, char* argv[]) {

	std::cout << "Start initializing lexical analyzer and syntax analyzer." << std::endl;
	SyntaxAnalyzer syntaxAnalyzer;
	LexicalAnalyzer lexicalAnalyzer;
	HtmlPrinter::Init("../output/result.html");
	lexicalAnalyzer.Init("../input/rule.in", "../input/formula.in");
	syntaxAnalyzer.Init("../input/sentence.in", "../input/map.in", "../input/LLTable.in");
	std::cout << "Init done." << std::endl;

	std::cout << "Start derivating." << std::endl;
	int token;
	std::string lexname;
	int cnt = 0;
	while (cnt < 2 && !lexicalAnalyzer.GetNextToken(token, lexname)) {
		int rtn;
		rtn = syntaxAnalyzer.MatchToken(token);
		if (token == ID || token == NUMBER || token == BLANK)
			syntaxAnalyzer.AssignLexname(token != BLANK ? lexname : std::string(" "));
		cnt += token == DOLLAR;
	}
	std::cout << "The tokens read in have been recorded in \"output/token.out\"." << std::endl;
	std::cout << "The derivations used have been recorded in \"output/derivation.out\"." << std::endl;
	std::cout << "Derivation done." << std::endl;

	std::cout << "Start calc the height of the grammar tree." << std::endl;
	syntaxAnalyzer.DetermineDepth();
	std::cout << "Height calculation done." << std::endl;

	std::cout << "Start printing formula." << std::endl;
	syntaxAnalyzer.Print();
	HtmlPrinter::Finish();
	std::cout << "The result has been written in \"output/result.html\"." << std::endl;
	std::cout << "Finished." << std::endl;
	return 0;
}
#include <bits/stdc++.h>
#include "source.h"
#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include "html_printer.h"

int main(int argc, char* argv[]) {

	std::cout << "Start initializing lexical analyzer." << std::endl;
	LexicalAnalyzer lexicalAnalyzer;
	lexicalAnalyzer.Init("../input/rule.in", "../input/formula.in");
	std::cout << "Lexical analyzer initialization done.\n" << std::endl;

	std::cout << "Start initializing syntax analyzer." << std::endl;
	SyntaxAnalyzer syntaxAnalyzer;
	syntaxAnalyzer.Init("../input/sentence.in", "../input/map.in", "../input/LLTable.in");
	std::cout << "Syntax analyzer initialization done.\n" << std::endl;

	std::cout << "Start initializing html printer." << std::endl;
	HtmlPrinter::Init("../output/result.html");
	std::cout << "Html printer initialization donw.\n" << std::endl;

	std::cout << "Start derivating." << std::endl << std::endl;
	int token;
	std::string lexname;
	int cnt = 0;
	bool lexicalError = false;
	bool syntaxError = false;
	while (cnt < 2) {
		int rtn;
		rtn = lexicalAnalyzer.GetNextToken(token, lexname);
		if (rtn == END_OF_FILE) {
			break;
		}
		if (rtn == INVALID_TOKEN) {
			lexicalError = true;
			continue;
		}
		rtn = syntaxAnalyzer.MatchToken(token);
		if (token == ID || token == NUMBER || token == BLANK)
			syntaxAnalyzer.AssignLexname(token != BLANK ? lexname : std::string(" "));
		cnt += token == DOLLAR;
	}
	std::cout << "The tokens read in have been recorded in \"output/token.out\"." << std::endl;
	std::cout << "The derivations used have been recorded in \"output/derivation.out\".\n" << std::endl;

	if (cnt < 2) {
		std::cout << "syntax_analyzer:\terror" << ": ";
		std::cout << "The formula is incomplete." << std::endl;
		syntaxError = true;
	}
	if (lexicalError || syntaxError) {
		std::cout << "Derivation terminated with error(s).\n" << std::endl;
		return 0;
	}

	std::cout << "Derivation done." << std::endl << std::endl;

	std::cout << "Start calc the height of the grammar tree." << std::endl;
	syntaxAnalyzer.DetermineDepth();
	std::cout << "Height calculation done." << std::endl << std::endl;

	std::cout << "Start printing formula." << std::endl;
	syntaxAnalyzer.Print();
	HtmlPrinter::Finish();
	std::cout << "The result has been written in \"output/result.html\"." << std::endl;
	std::cout << "Finished." << std::endl;
	return 0;
}
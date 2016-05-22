#include <bits/stdc++.h>
#include <unistd.h>
#include "source.h"
#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include "html_printer.h"

int main(int argc, char* argv[]) {

	std::cout << "Start initializing lexical analyzer." << std::endl;
	LexicalAnalyzer lexicalAnalyzer;
	lexicalAnalyzer.Init("../input/rule.in", "../input/formula.in");
	sleep(1);
	std::cout << "Lexical analyzer initialization complete.\n" << std::endl;
	sleep(1);

	std::cout << "Start initializing syntax analyzer." << std::endl;
	SyntaxAnalyzer syntaxAnalyzer;
	syntaxAnalyzer.Init("../input/sentence.in", "../input/map.in", "../input/LLTable.in");
	sleep(1);
	std::cout << "Syntax analyzer initialization complete.\n" << std::endl;
	sleep(1);

	std::cout << "Start initializing html printer." << std::endl;
	HtmlPrinter::Init("../output/result.html");
	sleep(1);
	std::cout << "Html printer initialization complete.\n" << std::endl;

	std::cout << "The tokens read in will be recorded in \"output/token.out\"." << std::endl;
	std::cout << "The derivations used will be recorded in \"output/derivation.out\".\n" << std::endl;

	std::cout << "Start derivating." << std::endl << std::endl;
	sleep(1);
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
		if (rtn == SUCC) {
			if (token == ID || token == NUMBER || token == BLANK)
				syntaxAnalyzer.AssignLexname(token != BLANK ? lexname : std::string(" "));
		} else {
			syntaxError = false;
		}
		cnt += token == DOLLAR;
	}

	if (cnt < 2) {
		std::cout << "syntax_analyzer:\terror" << ": ";
		std::cout << "The formula is incomplete." << std::endl;
		syntaxError = true;
	}
	if (lexicalError || syntaxError) {
		std::cout << "Derivation terminated with error(s).\n" << std::endl;
		return 0;
	}

	std::cout << "Derivation completed." << std::endl << std::endl;
	sleep(1);

	std::cout << "Start calc the height of the grammar tree." << std::endl;
	syntaxAnalyzer.DetermineDepth();
	sleep(1);
	std::cout << "Height calculation complete.\n" << std::endl;
	sleep(1);

	std::cout << "Start printing formula." << std::endl;
	syntaxAnalyzer.Print();
	HtmlPrinter::Finish();
	sleep(1);
	std::cout << "The result has been written in \"output/result.html\".\n" << std::endl;

	std::cout << "Formula translation complete." << std::endl;
	return 0;
}
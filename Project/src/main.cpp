#include <bits/stdc++.h>
#include "source.h"
#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include "html_printer.h"
#include "error_reporter.h"

int main(int argc, char* argv[]) {

	char* formulaInputFile;

	if (argc == 1) {
		std::cout << "Please input the formula code:" << std::endl;
		std::cout << "(input should be ended with #)" << std::endl;
		FILE* fp = fopen("../input/formula.in", "w");
		char ch;
		while ((ch = (char)getchar()) != '#') {
			if (ch != '\n') {
				fputc(ch, fp);
			}
		}
		fclose(fp);
		std::cout << "\n\n";
		formulaInputFile = (char *)"../input/formula.in";
	} else {
		formulaInputFile = argv[1];
	}

	std::string logMesgFile;

	std::cout << "Start initializing lexical analyzer." << std::endl;
	LexicalAnalyzer lexicalAnalyzer;
	lexicalAnalyzer.Init("../input/rule.in", formulaInputFile, logMesgFile);
	std::cout << "Lexical analyzer initialization complete.\n" << std::endl;

	std::cout << "The tokens read in will be recorded in \"output/token.out\"." << std::endl;

	std::cout << "Start initializing syntax analyzer." << std::endl;
	SyntaxAnalyzer syntaxAnalyzer;
	syntaxAnalyzer.Init("../input/sentence.in", "../input/map.in", "../input/LLTable.in", logMesgFile);
	std::cout << "Syntax analyzer initialization complete.\n" << std::endl;
	
	std::cout << "The derivations used will be recorded in \"" << logMesgFile << "\".\n" << std::endl;

	std::cout << "Start initializing html printer." << std::endl;
	HtmlPrinter::Init("../output/result.html");
	std::cout << "Html printer initialization complete.\n" << std::endl;

	std::cout << "Bind formula string to error reporter." << std::endl;
	ErrorReporter::Init(lexicalAnalyzer.FormulaPointer());
	std::cout << "Formula string successfully binded to error reporter.\n" << std::endl;

	std::cout << "===================Start derivating.===================" << std::endl << std::endl;
	int token;
	std::string lexname;
	bool lexicalError = false;
	bool syntaxError = false;
	while (true) {
		// return value
		int rtn;

		// fetch another token
		rtn = lexicalAnalyzer.GetNextToken(token, lexname);
		// if cannot fetch another token
		// the derivation must be incomplete at that time
		// otherwise it would have breaked
		// so there's only one possibility that the formula is incomplete
		if (rtn == END_OF_FILE) {
			std::cout << "lexical_analyzer: error" << ": ";
			std::cout << "The formula should be ended with $.\n" << std::endl;
			lexicalError = true;
			break;
		}
		if (rtn == INVALID_TOKEN) {
			lexicalError = true;
			continue;
		}

		rtn = syntaxAnalyzer.MatchToken(token, lexname);
		if (rtn == DRVT_COMPLETE) {
			break;
		}
		if (rtn == DRVT_TERMINATED) {
			std::cout << "syntax_analyzer: error" << ": ";
			std::cout << "The formula should be ended with $.\n" << std::endl;
			syntaxError = true;
			break;
		}
		if (rtn == MISMATCH_TOKEN) {
			syntaxError = true;
		}
	}

	if (lexicalError || syntaxError) {
		std::cout << "==========Derivation terminated with error(s).=========\n" << std::endl;
		return 0;
	} else {
		std::cout << "No error detected.\n" << std::endl;
	}

	std::cout << "=================Derivation completed.=================\n" << std::endl;

	std::cout << "Start calc the height of the grammar tree." << std::endl;
	syntaxAnalyzer.DetermineDepth();
	std::cout << "Height calculation complete.\n" << std::endl;

	std::cout << "Start printing formula." << std::endl;
	syntaxAnalyzer.Print();
	HtmlPrinter::Finish();
	std::cout << "The result has been written in \"output/result.html\".\n" << std::endl;

	std::cout << "Formula translation complete." << std::endl;
	return 0;
}
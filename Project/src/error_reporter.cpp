#include <bits/stdc++.h>
#include "error_reporter.h"

void ErrorReporter::Error(std::string& formula, std::string& token) {
	std::cout << "lexical_analyzer:\terror" << ": ";
	std::cout << "Invalid token \"" << token << "\" detected." << std::endl;
	std::cout << "\t" << formula << token << "\n\t";
	for (int i = 0; i < formula.size(); i++)
		std::cout << " ";
	std::cout << "^\n\n";
}
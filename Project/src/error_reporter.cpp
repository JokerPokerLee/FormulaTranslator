#include <bits/stdc++.h>
#include "error_reporter.h"

std::string* ErrorReporter::formula;

void ErrorReporter::PrintPosition(int tokenLength) {
	std::cout << "\t" << *formula << "\n\t";
	for (int i = 0; i < (*formula).size() - tokenLength; i++)
		std::cout << " ";
	std::cout << "^\n\n";
}
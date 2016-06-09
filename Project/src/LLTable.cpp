#include <bits/stdc++.h>
#include "source.h"
#include "LLTable.h"

void LLTable::Init(const char* LLTableInput) {
	// read in LL table
	FILE* fp = fopen(LLTableInput, "r");
	if (fp == NULL) {
		// File ERROR
	}
	int mark, token, derivation;
	while (fscanf(fp, "%d %d %d", &mark, &token, &derivation) != EOF) {
		std::pair<int , int> mtp = std::make_pair(mark, token);
		if (table.count(mtp) && table[mtp] != derivation) {
			std::cout << "Collision rules in LL table." << std::endl;
			return;
		}
		table[mtp] = derivation;
	}
	fclose(fp);
}

// query LL table term
int LLTable::Derivate(int mark, int token) {
	std::pair<int , int> mtp = std::make_pair(mark, token);
	if (table.count(mtp)) {
		return table[mtp];
	}
	return NO_LL_RULE;
}
#ifndef _LLTABLE_
#define _LLTABLE_

struct LLTable {
	// given mark-token pair and return action
	std::map<std::pair<int , int>, int> table;

	// build the table from LLTableInput
	// LLTableInput contains several line of the format:
	// mark token action
	void Init(const char* LLTableInput);
	// given mark-token pair and return action
	int Derivate(int mark, int token);
};

#endif
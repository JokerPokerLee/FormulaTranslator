#include <bits/stdc++.h>
#include "source.h"
#include "tokenDFA.h"

void TokenDFA::Init() {
	totalState = 0;
	startSta = 0;
	foundSta.clear();
	link.clear();
}

int TokenDFA::InsertRule(int state1, int state2, char ch) {
	if (!link.count(state1)
		|| !link[state1].count(ch)) {
		// 1. no such state found before
		//    must be no error
		// 2. state found before (previous condition false)
		//    and no such rule
		link[state1][ch] = state2;
		return SUCC;
	}
	// duplicated rule just ignore
	if (link[state1][ch] == state2) {
		return SUCC;
	}
	// collision detected
	std::cout << "Collisional rules in TokenDFA." << std::endl;
	std::cout << state1 << "--(" << ch << ")-->" << state2 << std::endl;
	std::cout << std::endl;
	return COLLIDE_RULE;
}

int TokenDFA::Next(char ch) {
	if (link[currentSta].count(ch)) {
		currentSta = link[currentSta][ch];
		return SUCC;
	}
	return INVALID_TOKEN;
}

bool TokenDFA::FoundToken() {
	return foundSta.count(currentSta);
}

void TokenDFA::Print() {
	for (int i = 0; i < totalState; i++)
		for (int j = 0; j < 256; j++) {
			if (link[i].count((char)j))
				std::cout << i << " " << link[i][(char)j] << " " << (char)j << std::endl;
		}
}
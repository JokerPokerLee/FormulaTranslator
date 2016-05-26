#include <bits/stdc++.h>
#include "source.h"
#include "tokenDFA.h"

void TokenDFA::Init() {
	totalState = 0;
	startSta = 0;
	foundSta.clear();
	link.clear();
}

void TokenDFA::InsertRule(int state1, int state2, char ch) {
	totalState = std::max(totalState, std::max(state1 + 1, state2 + 1));
	link.resize(totalState);
	if (link[state1].count(ch)) {
		std::cout << "Collisional rules in TokenDFA." << std::endl;
		exit(-1);
	}
	link[state1][ch] = state2;
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
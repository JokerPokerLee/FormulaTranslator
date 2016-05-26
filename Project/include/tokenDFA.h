#ifndef _TOKENDFA_
#define _TOKENDFA_

struct TokenDFA {
	int totalState;
	int startSta;
	int currentSta;
	std::map<int , int> foundSta;
	std::vector< std::map<char , int> > link;

	void Init();
	void InsertRule(int state1, int state2, char ch);
	// walk along the dfa.
	// if there is no path state remain still
	// return SUCC if path exist, INVALID_TOKEN otherwise
	int Next(char ch);
	bool FoundToken();
	void Print();
};

#endif
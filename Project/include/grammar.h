#ifndef _GRAMMAR_
#define _GRAMMAR_

struct Grammar {
	// the number of the derivation
	int derivationNumber;
	// store the derivation sign by sign
	// the first sign is the left value
	std::vector< std::vector<int> > derivation;
	// used for restore the derivation from token string
	// return the lexname of given token number
	std::map<int , std::string> translate;
	// debug output stream
	std::ofstream debugOutStream;

	// read in the derivation from grammarInput
	// read in the token-lex map
	int Init(const char* grammarInput, const char* mapInput);
	// return the i-th sign in k-th derivation
	int FetchSign(int k, unsigned int i);
	// print k-th derivations
	void Print(int k);
};

#endif
#include <bits/stdc++.h>
#include "source.h"
#include "grammar.h"

int Grammar::Init(const char* grammarInput, const char* mapInput) {
	derivationNumber = 0;
	derivation.clear();
	FILE* fp = fopen(grammarInput, "r");
	while (true) {
		std::string currentSentence = "";
		char ch;
		// use the \n and EOF to seperate each derivation
		while ((ch = fgetc(fp)) != '\n' && ch != EOF)
			currentSentence += ch;
		if (currentSentence.size() == 0) break;
		derivationNumber++;
		derivation.resize(derivationNumber);
		for (unsigned int i = 0; i < currentSentence.size(); i++) {
			int code;
			// rematch the signs with single char
			switch (currentSentence[i]) {
				case 'S': code = SMARK;			break;
				case 'B': code = BMARK;			break;
				case 'F': code = FMARK;			break;
				case '$': code = DOLLAR;		break;
				case '&': code = ALLSCRIPT;		break;
				case '^': code = SUPERSCRIPT;	break;
				case '_': code = SUBSCRIPT;		break;
				case 'i': code = INTEGRITY;		break;
				case 's': code = SIGMA;			break;
				case 'd': code = ID;			break;
				case 'n': code = NUMBER;		break;
				case 'b': code = BLANK;			break;
				case '(': code = LEFTBRACKET;	break;
				case ')': code = RIGHTBRACKET;	break;
				case '{': code = LEFTCURLY;		break;
				case '}': code = RIGHTCURLY;	break;
				case 'e': code = EMPTY;			break;
			}
			derivation.back().push_back(code);
		}
	}
	fclose(fp);

	// to generate readable debug for grammar init
	fp = fopen(mapInput, "r");
	int sign;
	char* lexname = new char[10];
	while (fscanf(fp, "%d %s", &sign, lexname) != EOF) {
		translate[sign] = std::string(lexname);
	}
	delete lexname;
	fclose(fp);

	// output all the derivation uesd in process
	debugOutStream.open("../log/derivation.out");
	if (debugOutStream.fail()) {
		std::cout << "Can't not open Project/log/derivation.out file." << std::endl;
		std::cout << "Simply output the derivation info in the current directory.\n" << std::endl;
		debugOutStream.open("./derivation.out");
		debugOutStream << "Left most derivation:" << std::endl;
		return LOG_FILE_FAIL;
	}
	debugOutStream << "Left most derivation:" << std::endl;
	return SUCC;
}

int Grammar::FetchSign(int k, unsigned int i) {
	// no such sentence or exceed the sentence length
	if (k >= derivationNumber || i >= derivation[k].size()) {
		return EXCEED_DRVT;
	}
	// F -> emplty
	if (derivation[k][i] == EMPTY) return EXCEED_DRVT;
	return derivation[k][i];
}

void Grammar::Print(int k) {
	debugOutStream << translate[derivation[k][0]] << " -> ";
	for (unsigned int i = 1; i < derivation[k].size(); i++) {
		debugOutStream << translate[derivation[k][i]];
	}
	debugOutStream << std::endl;
}
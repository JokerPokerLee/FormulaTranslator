#include <bits/stdc++.h>
#include "source.h"
#include "html_printer.h"
#include "syntax_analyzer.h"

void Grammar::Init(const char* grammarInput, const char* mapInput) {
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
		for (int i = 0; i < currentSentence.size(); i++) {
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

	std::cout << "derivation init done" << std::endl;

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
	debugOutStream.open("../output/derivation.out");
}

int Grammar::FetchSign(int k, int i) {
	if (k >= derivationNumber || i >= derivation[k].size()) {
		return EXCEED_DRVT;
	}
	if (derivation[k][i] == EMPTY) return EXCEED_DRVT;
	return derivation[k][i];
}

void Grammar::Print(int k) {
	debugOutStream << translate[derivation[k][0]] << " -> ";
	for (int i = 1; i < derivation[k].size(); i++) {
		debugOutStream << translate[derivation[k][i]];
	}
	debugOutStream << std::endl;
}

void LLTable::Init(const char* LLTableInput) {
	FILE* fp = fopen(LLTableInput, "r");
	int mark, token, derivation;
	while (fscanf(fp, "%d %d %d", &mark, &token, &derivation) != EOF) {
		std::pair<int , int> mtp = std::make_pair(mark, token);
		if (table.count(mtp) && table[mtp] != derivation) {
			std::cout << "Collision rule in LL table." << std::endl;
			return;
		}
		table[mtp] = derivation;
	}
	fclose(fp);
}

int LLTable::Derivate(int mark, int token) {
	std::pair<int , int> mtp = std::make_pair(mark, token);
	if (table.count(mtp)) {
		return table[mtp];
	}
	return NO_LL_RULE;
}

void SyntaxAnalyzer::Init(const char* grammarInput, const char* mapInput, const char* LLTableInput) {
	grammar.Init(grammarInput, mapInput);
	table.Init(LLTableInput);
	grammar.Print(1);
	mCurrentNode = root = new Node(1, NULL);
}

int SyntaxAnalyzer::MatchToken(int token) {
	while (true) {
		// sign stands for the expect sign in LL derivation
		int sign = grammar.FetchSign(mCurrentNode -> type, mCurrentNode -> matchCursor);
		// matching process in current derivation is done
		if (sign == EXCEED_DRVT) {
			mCurrentNode = mCurrentNode -> parent;
			if (mCurrentNode != NULL) {
				mCurrentNode -> matchCursor++;
				continue;
			} else {
				return DRVT_COMPLETE;
			}
		}
		// if match, push cursor
		if (token == sign) {
			mCurrentNode -> matchCursor++;
			return TOKEN_MATCH;
		}
		// if the expect sign is a non-terminal token, derivate recursively
		if (sign >= 2000) {
			int derivation = table.Derivate(sign, token);
			// no match rule
			if (derivation == NO_LL_RULE) {}
			// used for debug
			grammar.Print(derivation);
			mCurrentNode -> next.push_back(new Node(derivation, mCurrentNode));
			mCurrentNode = mCurrentNode -> next.back();
		} else {
			return MISMATCH_TOKEN;
		}
	}
	return SUCC;
}

void SyntaxAnalyzer::AssignLexname(std::string lexname) {
	mCurrentNode -> lexname = new std::string(lexname);
}

void SyntaxAnalyzer::DetermineDepth(Node* node) {
	if (node -> lexname != NULL) {
		node -> fontSize = 10;
		return;
	}
	for (int i = 0; i < node -> next.size(); i++) {
		DetermineDepth(node -> next[i]);
		int ratio = 0;
		if (node -> type >= 4 && node -> type <= 6)
			ratio = 1;
		if (node -> type >= 7 && node -> type <= 8 && i < 4)
			ratio = 1;
		node -> fontSize = std::max(node -> fontSize, node -> next[i] -> fontSize + 10 * ratio);
	}
}

void SyntaxAnalyzer::DetermineDepth() {
	DetermineDepth(root);
	std::queue<Node*> nodeQueue;
	nodeQueue.push(root);
	while (nodeQueue.size()) {
		Node* now = nodeQueue.front();
		nodeQueue.pop();
		for (int i = 0; i < now -> next.size(); i++) {
			int ratio = 0;
			if (now -> type >= 4 && now -> type <= 6)
				ratio = 1;
			if (now -> type >= 7 && now -> type <= 8 && i < 4)
				ratio = 1;
			now -> next[i] -> fontSize = now -> fontSize - 10 * ratio;
			nodeQueue.push(now -> next[i]);
		}
	}
}

int SyntaxAnalyzer::PrintOneScript(int pos, Node* currentNode, int nextNumber) {
	//current info tmp
	int currentTop = currentNode -> top;
	int currentCursor = currentNode -> printCursor;
	int currentSize = currentNode -> fontSize;

	//next info tmp
	int nextSize = currentSize - 10;
	Node* nextNode = currentNode -> next[nextNumber];
	// pos = 0 when solve superscript vise verse subscript pos = 1
	int nextTop = currentTop + (nextSize - pos * currentSize * 2) * 3 / 5;

	nextNode -> SetPosition(currentCursor, nextTop);
	return PrintSentence(nextNode);
}

int SyntaxAnalyzer::PrintAllScript(Node* currentNode) {
	int currentTop = currentNode -> top;
	int originCursor = currentNode -> printCursor;
	int& currentCursor = currentNode -> printCursor;
	//print subscript
	currentCursor = originCursor;
	currentCursor = PrintOneScript(1, currentNode, 0);
	currentCursor = PrintOneScript(1, currentNode, 1);
	//print superscript
	currentCursor = originCursor;
	currentCursor = PrintOneScript(0, currentNode, 2);
	currentCursor = PrintOneScript(0, currentNode, 3);
	//push cursor and print suffix
	currentCursor = std::max(currentNode -> next[1] -> printCursor, currentNode -> next[3] -> printCursor);
	currentNode -> next[4] -> SetPosition(currentCursor, currentTop);
	currentCursor = PrintSentence(currentNode -> next[4]);
	if (currentNode -> type >= 7) {
		currentNode -> next[5] -> SetPosition(currentCursor, currentTop);
		currentCursor = PrintSentence(currentNode -> next[5]);
	}
	return currentCursor;
}

int SyntaxAnalyzer::PrintSentence(Node* currentNode) {
	int currentTop = currentNode -> top;
	int currentType = currentNode -> type;
	int currentSize = currentNode -> fontSize;
	int& currentCursor = currentNode -> printCursor;
	if (currentType == 3)
		return currentCursor;
	if (currentType >= 9 && currentType <= 11) {
		std::string token = *(currentNode -> lexname);
		int style = currentType == ID ? 1 : 0;
		currentCursor = HtmlPrinter::PrintToken(currentCursor, currentTop, currentSize, style, token);
		return currentNode -> printCursor;
	}
	int pos;
	std::string token;
	switch (currentNode -> type) {
		case 1:
		case 2:
			for (int i = 0; i < currentNode -> next.size(); i++) {
				currentNode -> next[i] -> SetPosition(currentCursor, currentTop);
				currentCursor = PrintSentence(currentNode -> next[i]);
			}
			break;
		case 4:
			currentCursor = PrintAllScript(currentNode);
			break;
		case 5:	
		case 6:
			pos = (currentNode -> type + 1) & 1;
			currentCursor = PrintOneScript(pos, currentNode, 0);
			currentCursor = PrintOneScript(pos, currentNode, 1);
			currentNode -> next[2] -> SetPosition(currentCursor, currentTop);
			currentCursor = PrintSentence(currentNode -> next[2]);
			break;
		case 7:
		case 8:
			token = (currentNode -> type & 1) ? "∫" : "∑";
			currentCursor = HtmlPrinter::PrintToken(currentCursor, currentTop, currentSize, 0, token);
			currentCursor = PrintAllScript(currentNode);
			break;
		case 12:
			currentCursor = HtmlPrinter::PrintToken(currentCursor, currentTop, currentSize, 0, std::string("("));
			for (int i = 0; i < currentNode -> next.size(); i++) {
				currentNode -> next[i] -> SetPosition(currentCursor, currentTop);
				currentCursor = PrintSentence(currentNode -> next[i]);
			}
			currentCursor = HtmlPrinter::PrintToken(currentCursor, currentTop, currentSize, 0, std::string(")"));
			break;
		default:
			//ERROR
			break;
	}
	return currentNode -> printCursor;
}

void SyntaxAnalyzer::Print() {
	root -> printCursor = 100;
	root -> top = 300;
	PrintSentence(root);
}
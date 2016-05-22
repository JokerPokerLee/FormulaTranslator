#include <bits/stdc++.h>
#include "source.h"
#include "grammar.h"
#include "LLTable.h"
#include "html_printer.h"
#include "syntax_analyzer.h"

static std::string leftCurly = std::string("(");
static std::string rightCurly = std::string(")");

void SyntaxAnalyzer::Init(const char* grammarInput, const char* mapInput, const char* LLTableInput) {
	grammar.Init(grammarInput, mapInput);
	table.Init(LLTableInput);
	// first derivation
	grammar.Print(1);
	mCurrentNode = root = new Node(1, NULL);
}

// read in token and decide to derivate or move on
int SyntaxAnalyzer::MatchToken(int token) {
	while (true) {
		// sign stands for the expect sign in LL derivation
		int sign = grammar.FetchSign(mCurrentNode -> type, mCurrentNode -> matchCursor);
		// matching process in current sentence is done
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
		if (sign < 2000) {
			if (token == sign) {
				mCurrentNode -> matchCursor++;
				return SUCC;
			} else {
				
			}
		}
		// if the expect sign is a non-terminal token, derivate recursively
		if (sign >= 2000) {
			int derivation = table.Derivate(sign, token);
			// no match rule
			if (derivation == NO_LL_RULE) {
				return MISMATCH_TOKEN;
			}
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

// only leaf node can have lexname
void SyntaxAnalyzer::AssignLexname(std::string lexname) {
	// lexname is a pointer to string
	// must assign a new storage area
	mCurrentNode -> lexname = new std::string(lexname);
}

// recursive determine minimum size
void SyntaxAnalyzer::DetermineDepth(Node* node) {
	// default font size
	node -> fontSize = MINIMUM_SIZE;
	for (int i = 0; i < node -> next.size(); i++) {
		DetermineDepth(node -> next[i]);
		// indicate whether to shrink
		int ratio = 0;
		// have only supscript or subscript
		if (node -> type >= 5 && node -> type <= 6)
			ratio = i < 2;
		// have both supscript and subscript
		if (node -> type == 4 || node -> type >= 7 && node -> type <= 8)
			ratio = i < 4;
		node -> fontSize = std::max(node -> fontSize, node -> next[i] -> fontSize * (ratio + 1));
	}
}

// first find the minimum
// second adjust the minimum
// third update descendant size from root use bfs
void SyntaxAnalyzer::DetermineDepth() {
	// calc least need font size of root
	DetermineDepth(root);
	// minimum root node font size;
	root -> fontSize = std::max(root -> fontSize, MINIMUM_ROOT);
	// use bfs method to update descendant font size
	std::queue<Node*> nodeQueue;
	nodeQueue.push(root);
	while (nodeQueue.size()) {
		Node* now = nodeQueue.front();
		nodeQueue.pop();
		for (int i = 0; i < now -> next.size(); i++) {
			// indicate whether to shrink
			int ratio = 0;
			// have only supscript or subscript
			if (now -> type >= 5 && now -> type <= 6)
				ratio = i < 2;
			// have both supscript and subscript
			if (now -> type == 4 || now -> type >= 7 && now -> type <= 8)
				ratio = i < 4;
			now -> next[i] -> fontSize = now -> fontSize / (ratio + 1);
			nodeQueue.push(now -> next[i]);
		}
	}

	//check result
}

// current node has only subscript or supscript
// pos indicate the sub(1) or the sup(0)
// print only script
int SyntaxAnalyzer::PrintOneScript(int pos, Node* currentNode, int nextNumber) {
	//current info tmp
	int currentTop = currentNode -> top;
	int currentCursor = currentNode -> printCursor;
	int currentSize = currentNode -> fontSize;

	//next info tmp
	int nextSize = currentSize / 2;
	Node* nextNode = currentNode -> next[nextNumber];
	// pos = 0 when solve superscript vise verse subscript pos = 1
	int nextTop = currentTop + pos * (currentSize - nextSize);
	nextTop += pos ? (nextSize / 5) : -(nextSize / 2);
	//nextTop += (pos ^ 1) * nextSize / 5;

	nextNode -> SetPosition(currentCursor, nextTop);
	return PrintSentence(nextNode);
}

// print scripts and others
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

// print all kinds of formulas
int SyntaxAnalyzer::PrintSentence(Node* currentNode) {
	// current node info tmp
	int currentTop = currentNode -> top;
	int currentType = currentNode -> type;
	int currentSize = currentNode -> fontSize;
	int& currentCursor = currentNode -> printCursor;
	// F -> empty
	if (currentType == 3) return currentCursor;
	// terminal tokens directly print
	if (currentType >= 9 && currentType <= 11) {
		std::string token = *(currentNode -> lexname);
		int style = currentType == 9 ? 1 : 0;
		currentCursor = HtmlPrinter::PrintToken(currentCursor, currentTop, currentSize, style, token, token.size());
		return currentNode -> printCursor;
	}
	int pos, delta;
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
			if (currentNode -> type & 1) {
				token = "∫";
				delta = 5;
			} else {
				token = "Σ";
				delta = 5;
			}
			// token = (currentNode -> type & 1) ? "∫" : "∑";
			currentCursor = HtmlPrinter::PrintToken(currentCursor, currentTop - delta * 4 / 5, currentSize + delta, 0, token, 1);
			currentCursor = PrintAllScript(currentNode);
			break;
		case 12:
			currentCursor = HtmlPrinter::PrintToken(currentCursor, currentTop, currentSize, 0, leftCurly, 1);
			for (int i = 0; i < currentNode -> next.size(); i++) {
				currentNode -> next[i] -> SetPosition(currentCursor, currentTop);
				currentCursor = PrintSentence(currentNode -> next[i]);
			}
			currentCursor = HtmlPrinter::PrintToken(currentCursor, currentTop, currentSize, 0, rightCurly, 1);
			break;
		default:
			//ERROR
			break;
	}
	return currentNode -> printCursor;
}

// outer interface
void SyntaxAnalyzer::Print() {
	// initialize root position
	root -> printCursor = 100;
	root -> top = 300;
	PrintSentence(root);
}
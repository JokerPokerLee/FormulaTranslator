#include <bits/stdc++.h>
#include "html_printer.h"

FILE* HtmlPrinter::htmlOutputFile;

void HtmlPrinter::Init(const char* formulaOutput) {
	htmlOutputFile = fopen(formulaOutput, "w");
	fprintf(htmlOutputFile, "<html>\n");
	fprintf(htmlOutputFile, "\t<head>\n");
	fprintf(htmlOutputFile, "\t\t<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\" />\n");
	fprintf(htmlOutputFile, "\t</head>\n\t<body>\n");
}

int HtmlPrinter::PrintToken(int left, int top, int fontSize, int fontStyle, std::string& token, int len) {
	// continuous ID need to be separated by space
	// a token with leading space mark the situation above
	if (token[0] == ' ') {
		left += fontSize * 2 / 5;
		token = token.substr(1, token.size() - 1);
		len--;
	}
	fprintf(htmlOutputFile, "\t\t<div style=\"position: absolute; left:%dpx; top:%dpx;\">\n", left, top);
	fprintf(htmlOutputFile, "\t\t\t<span style=\"font-family:%s; font-size:%dpx; font-style:%s; line-height:100%%;\">\n", FONT_FAMILY, fontSize, fontStyle ? "oblique" : "normal");
	fprintf(htmlOutputFile, "\t\t\t\t%s\n\t\t\t</span>\n\t\t</div>\n", token.c_str());
	return left + len * fontSize * 3 / 5;
}

void HtmlPrinter::Finish() {
	fprintf(htmlOutputFile, "\t</body>\n</html>\n");
	fclose(htmlOutputFile);
}
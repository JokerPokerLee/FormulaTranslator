#include <bits/stdc++.h>
#include "html_printer.h"

FILE* HtmlPrinter::htmlOutputFile;

void HtmlPrinter::Init(const char* formulaOutput) {
	htmlOutputFile = fopen(formulaOutput, "w");
	fprintf(htmlOutputFile, "<html>\n");
	fprintf(htmlOutputFile, "\t<head>\n");
	fprintf(htmlOutputFile, "\t\t<META content=\"text/html; charset=gb2312\">\n");
	fprintf(htmlOutputFile, "\t</head>\n\t<body>\n");
}

int HtmlPrinter::PrintToken(int left, int top, int fontSize, int fontStyle, std::string& token, int len) {
	fprintf(htmlOutputFile, "\t\t<div style=\"position: absolute; left:%dpx; top:%dpx;\">\n", left, top);
	fprintf(htmlOutputFile, "\t\t\t<span style=\"font-family:monospace; font-size:%dpx; font-style:%s; line-height:100%%;\">", fontSize, fontStyle ? "oblique" : "normal");
	fprintf(htmlOutputFile, "%s</span>\n\t\t</div>\n", token.c_str());
	return left + len * fontSize * 3 / 5;
}

void HtmlPrinter::Finish() {
	fprintf(htmlOutputFile, "\t</body>\n</html>\n");
	fclose(htmlOutputFile);
}
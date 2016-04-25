#ifndef _HTML_PRINTER_
#define _HTML_PRINTER_

class HtmlPrinter {
private:
	static FILE* htmlOutputFile;
public:
	static void Init(const char* formulaOutput);
	//fontStyle = 0 when wants normal
	static int PrintToken(int left, int top, int fontSize, int fontStyle, std::string token);
	static void Finish();
};

#endif
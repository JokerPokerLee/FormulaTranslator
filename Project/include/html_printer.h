#ifndef _HTML_PRINTER_
#define _HTML_PRINTER_

#define FONT_FAMILY "Anonymous"

class HtmlPrinter {
private:
	static FILE* htmlOutputFile;
public:
	static void Init(const char* formulaOutput);
	//fontStyle = 0 when wants normal
	static int PrintToken(int left, int top, int fontSize, int fontStyle, std::string& token, int len);
	static void Finish();
};

#endif
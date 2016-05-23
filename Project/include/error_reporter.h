#ifndef _ERROR_REPORTER_
#define _ERROR_REPORTER_

class ErrorReporter {
private:
	static std::string* formula;
public:
	static void Init(std::string* formula_ptr) {
		formula = formula_ptr;
	};
	static void PrintPosition(int tokenLength);
};

#endif
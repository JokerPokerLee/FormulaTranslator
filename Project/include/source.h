#ifndef _SOURCE_H_
#define _SOURCE_H_

#define MINIMUM_SIZE	5
#define MINIMUM_ROOT	40

//[0000, 0100) succ type
#define SUCC			0000
#define DRVT_COMPLETE	0001

//[0100, 1000) error type
#define EXCEED_DRVT		0100
#define NO_LL_RULE		0101
#define MISMATCH_TOKEN	0102
#define INVALID_TOKEN	0103
#define END_OF_FILE		0104
#define DRVT_TERMINATED	0105
#define COLLIDE_RULE	0106
#define NO_FORMULA_FILE	0107
#define LOG_FILE_FAIL	0110

//[1000, 2000) token name
#define INTEGRITY		1000
#define SIGMA			1001
#define BLANK			1002
#define SUBSCRIPT		1003
#define ALLSCRIPT		1004
#define SUPERSCRIPT		1005
#define LEFTCURLY		1006
#define RIGHTCURLY		1007
#define LEFTBRACKET		1008
#define RIGHTBRACKET	1009
#define NUMBER			1010
#define ID				1011
#define DOLLAR			1012
#define EMPTY			1013

//[2000, 3000) mark name
#define SMARK			2000
#define FMARK			2001
#define BMARK			2002

#endif
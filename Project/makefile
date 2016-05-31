CC			=	g++ -Wall -std=c++11
CFLAGS		=	-I include/
TARGET		=	formula_translator
LEXANA		=	lexical_analyzer
TEXANA		=	syntax_analyzer
SPRINT		=	html_printer
SERROR		=	error_reporter
COMMON		=	include/source.h
COMMAND		=	$(CC) $(CFLAGS) -c $< -o $@

all: bin/$(TARGET)

bin/$(TARGET): lib/main.o lib/$(SERROR).o lib/$(LEXANA).o lib/tokenDFA.o lib/$(SPRINT).o lib/$(TEXANA).o lib/grammar.o lib/LLTable.o
	@mkdir -p bin
	@echo link all .o files
	@echo 1 > .compile_acted
	@$(CC) $(CFLAGS) $^ -o $@

lib/main.o: src/main.cpp include/*
	@mkdir -p lib
	@echo compiling main
	@$(COMMAND)

lib/$(SERROR).o: src/$(SERROR).cpp include/$(SERROR).h
	@echo compiling $(SERROR)
	@$(COMMAND)

lib/$(LEXANA).o: src/$(LEXANA).cpp include/$(LEXANA).h include/$(SERROR).h include/tokenDFA.h $(COMMON)
	@echo compiling $(LEXANA)
	@$(COMMAND)

lib/tokenDFA.o: src/tokenDFA.cpp include/tokenDFA.h $(COMMON)
	@echo compiling tokenDFA
	@$(COMMAND)

lib/$(SPRINT).o: src/$(SPRINT).cpp include/$(SPRINT).h
	@echo compiling $(SPRINT)
	@$(COMMAND)

lib/$(TEXANA).o: src/$(TEXANA).cpp include/$(TEXANA).h include/$(SPRINT).h include/grammar.h include/LLTable.h $(COMMON)
	@echo compiling $(TEXANA)
	@$(COMMAND)

lib/grammar.o: src/grammar.cpp include/grammar.h
	@echo compiling grammar
	@$(COMMAND)

lib/LLTable.o: src/LLTable.cpp include/LLTable.h
	@echo compiling LLTable
	@$(COMMAND)

.PHONY: clean
clean:
	@rm -rf lib
	@rm -rf bin

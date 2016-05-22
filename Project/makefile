CC			=	g++ -w -std=c++11
CFLAGS		=	-I include/
TARGET		=	formula_translator
LEXANA		=	lexical_analyzer
TEXANA		=	syntax_analyzer
SPRINT		=	html_printer
SERROR		=	error_reporter
COMMON		=	include/source.h

all: bin/$(TARGET)

bin/$(TARGET): lib/main.o lib/$(SERROR).o lib/$(LEXANA).o lib/$(SPRINT).o lib/$(TEXANA).o lib/grammar.o lib/LLTable.o
	@mkdir -p bin
	@echo link all .o files
	@echo 1 > .compile_acted
	@$(CC) $(CFLAGS) $^ -o $@

lib/main.o: src/main.cpp include/*
	@mkdir -p lib
	@echo compile main.o
	@$(CC) $(CFLAGS) -c $< -o $@

lib/$(SERROR).o: src/$(SERROR).cpp include/$(SERROR).h
	@mkdir -p lib 
	@echo compile $(SERROR).o
	@$(CC) $(CFLAGS) -c $< -o $@

lib/$(LEXANA).o: src/$(LEXANA).cpp include/$(LEXANA).h include/$(SERROR).h $(COMMON)
	@mkdir -p lib
	@echo compile $(LEXANA).o
	@$(CC) $(CFLAGS) -c $< -o $@

lib/$(SPRINT).o: src/$(SPRINT).cpp include/$(SPRINT).h
	@mkdir -p lib
	@echo compile $(SPRINT).o
	@$(CC) $(CFLAGS) -c $< -o $@

lib/$(TEXANA).o: src/$(TEXANA).cpp include/$(TEXANA).h include/$(SPRINT).h include/grammar.h include/LLTable.h $(COMMON)
	@mkdir -p lib
	@echo compile $(TEXANA).o
	@$(CC) $(CFLAGS) -c $< -o $@

lib/grammar.o: src/grammar.cpp include/grammar.h
	@mkdir -p lib
	@echo compile grammar.o
	@$(CC) $(CFLAGS) -c $< -o $@

lib/LLTable.o: src/LLTable.cpp include/LLTable.h
	@mkdir -p lib
	@echo compile LLTable.o
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@rm -rf lib
	@rm -rf bin

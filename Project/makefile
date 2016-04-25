CC			=	g++ -w -std=c++11
CFLAGS		=	-I include
TARGET		=	formula_translater
LEXANA		=	lexical_analyzer
TEXANA		=	syntax_analyzer
SPRINT		=	html_printer
COMMON		=	include/source.h

all: bin/formula_translater

bin/$(TARGET): lib/$(TARGET).o lib/$(LEXANA).o lib/$(SPRINT).o lib/$(TEXANA).o
	@mkdir -p bin
	@echo link all .o files
	@$(CC) $(CFLAGS) $^ -o $@

lib/$(TARGET).o: FormulaTranslater.cpp
	@mkdir -p lib
	@echo compile $(TARGET).o
	@$(CC) $(CFLAGS) -c $< -o $@

lib/$(LEXANA).o: src/$(LEXANA).cpp include/$(LEXANA).h $(COMMON)
	@mkdir -p bin
	@echo compile $(LEXANA).o
	@$(CC) $(CFLAGS) -c $< -o $@

lib/$(SPRINT).o: src/$(SPRINT).cpp include/$(SPRINT).h
	@mkdir -p bin
	@echo compile $(SPRINT).o
	@$(CC) $(CFLAGS) -c $< -o $@

lib/$(TEXANA).o: src/$(TEXANA).cpp include/$(TEXANA).h include/$(SPRINT).h $(COMMON)
	@mkdir -p bin
	@echo compile $(TEXANA).o
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf lib
	@rm -rf bin
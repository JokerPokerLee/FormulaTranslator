g++ -std=c++11 -I include/ -c src/lexical_analyzer.cpp -o lib/lexical_analyzer.o
g++ -std=c++11 -I include/ -c src/html_printer.cpp -o lib/html_printer.o
g++ -std=c++11 -I include/ -c src/syntax_analyzer.cpp -o lib/syntax_analyzer.o
g++ -std=c++11 -I include/ FormularTranslate.cpp lib/*.o -o bin/FormularTranslate

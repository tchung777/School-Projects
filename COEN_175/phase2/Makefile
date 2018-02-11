lexmake: lexer.l
	lex -t lexer.l > lexer.cpp
	g++ -o scc parser.cpp lexer.cpp

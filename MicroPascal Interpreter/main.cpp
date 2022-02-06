#include <iostream>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

int main(int argc, char const* argv[])
{
	try
	{
		std::string input =
			"program test; \n"
			"procedure q(a : integer);\n"
			"var i:integer;\n"
			"begin\n"
			"for i:=1 to a do\n"
			"begin\n"
			"p(1);\n"
			"writeln('b');\n"
			"end;\n"
			"end;\n"
			"procedure p(a : integer);\n"
			"var i:integer;\n"
			"begin\n"
			"for i:=1 to a do\n"
			"begin\n"
			"q(1);\n"
			"writeln('a');\n"
			"end;\n"
			"end;\n"
			"begin\n"
			"p(5);\n"
			"q(10);\n"
			"end.\n";

		Lexer lex(input);
		
		std::vector<Token> tokens = lex.GetTokens();

		/*/
		for (auto&& tok : tokens)
		{
			tok.Print();
		}
		/**/

		/*
		TODO:	
		o nacteni ze souboru
		o warningy pryc
		o code review a dokumentace
		o cmake
		*/

		Parser par(tokens);
		Interpreter interpreter;

		interpreter.Interpret(par.Parse());

		/*
		
		*/
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	
	return 0;
}
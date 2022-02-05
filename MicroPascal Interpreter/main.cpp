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
			"var a:integer;\n"
			"b:boolean;"
			"c:integer;"
			"begin		 \n"
			"writeln(a);\n"
			"writeln(b);\n"
			"writeln(c);\n"
			"end.\n";

		Lexer lex(input);
		std::vector<Token> tokens = lex.GetTokens();
		/*/
		for (auto&& tok : tokens)
		{
			tok.Print();
		}
		/**/

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
#include <iostream>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

int main(int argc, char const* argv[])
{
	try
	{
		std::string input =
			"program ; \n"
			"begin		 \n"
			"writeln(78965*458-887 div 3)\n"
			"writeln(78965*458-887 div 3)\n"
			"end.\n";

		Lexer lex(input);
		std::vector<Token> tokens = lex.GetTokens();

		Parser par(tokens);
		Interpreter interpreter;

		interpreter.Interpret(par.Parse());

		/*
		for (auto&& tok : tokens)
		{
			tok.Print();
		}
		*/
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	
	return 0;
}
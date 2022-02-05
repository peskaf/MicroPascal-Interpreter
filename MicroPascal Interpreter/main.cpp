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
			"begin		 \n"
			"writeln(a);\n"
			"a := 45 * 9 + a - 2 div 3;\n"
			"writeln(a);\n"
			"a := a - 404;\n"
			"writeln(a);\n"
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
		o Variable assignment
		o Flow control
		o Fnc/proc declaration
		o Fnc/Proc call
		o ENDE!!!!!!!!!!!
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
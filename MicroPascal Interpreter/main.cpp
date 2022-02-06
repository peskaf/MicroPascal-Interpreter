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
			"var i:integer;\n"
			"begin		 \n"
			"for i := 0 to 10 do begin if i <> (i div 2)*2 then writeln('i = ',i,' : odd'); end;\n"
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
		o Fnc/proc declaration
		o Fnc/Proc call
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
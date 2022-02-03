#include <iostream>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

int main(int argc, char const* argv[])
{
	try
	{
		std::string input = "'hello' + 'ggg'";

		Lexer lex(input);
		std::vector<Token> tokens = lex.GetTokens();

		Parser par(tokens);
		Interpreter interpreter;
		std::cout << std::get<std::string>(par.Parse()->Accept(interpreter)) << std::endl;

		for (auto&& tok : tokens)
		{
			tok.Print();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	
	return 0;
}
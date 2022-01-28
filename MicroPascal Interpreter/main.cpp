#include <iostream>

#include "Lexer.cpp"

int main(int argc, char const* argv[])
{
	try
	{
		Lexer lex("()**::=<>{123**}123    (5");
		std::list<Token> tokens = lex.GetTokens();

		for (auto&& tok : tokens)
		{
			tok.Print();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	
	/*
	if (argc > 2) // wrong usage
	{
		std::cout << "Usage: micropascal [script]" << std::endl; // edit later
		std::exit(64);
	}
	else if (argc == 2)
	{
		// run from file args[1]
	}
	else
	{
		// run prompt
	}
	*/
	return 0;
}
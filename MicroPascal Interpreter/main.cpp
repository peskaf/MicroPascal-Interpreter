#include <iostream>

#include "Lexer.cpp"

int main(int argc, char const* argv[])
{
	try
	{
		Lexer lex("12 34 ;		\n\n\n := <><=< {123 \n*()\n133 ahoj})*");
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
	
	return 0;
}
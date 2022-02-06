#include <iostream>
#include <fstream>
#include <sstream>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

int main(int argc, char const* argv[])
{
	try
	{
		std::string input;

		if (argc == 2) // file
		{
			std::stringstream ss;
			// check file errors
			std::ifstream file(argv[1]);
			std::string temp;
			while (std::getline(file, temp)) {
				ss << temp << '\n';
			}
			input = ss.str();
		}

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
		o warningy pryc
		o code review a dokumentace
		o cmake
		*/

		Parser par(tokens);
		Interpreter interpreter;

		interpreter.Interpret(par.Parse());
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	
	return 0;
}
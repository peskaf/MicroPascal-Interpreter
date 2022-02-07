#include <iostream>
#include <fstream>
#include <sstream>

#include "Error.hpp"
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

		/*
		TODO:
		o code review a dokumentace
		o cmake
		*/

		Parser par(tokens);
		Interpreter interpreter;

		interpreter.Interpret(par.Parse());
	}
	catch (const Error& e)
	{
		std::cout << e.what() << std::endl;
	}
	
	return 0;
}
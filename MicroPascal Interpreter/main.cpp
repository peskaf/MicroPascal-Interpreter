#include <iostream>
#include <fstream>
#include <sstream>

#include "Error.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

int main(int argc, char const* argv[])
{
	std::string input;
	std::stringstream ss;

	// read input
	if (argc > 2)
	{
		std::cout << "Usage: mpas [script]" << std::endl;
		return 1;
	}
	else if (argc == 2) // from file
	{
		try
		{
			std::ifstream file(argv[1]);
			std::string line;
			while (std::getline(file, line)) {
				ss << line << '\n';
			}
		}
		catch (const std::exception&)
		{
			std::cerr << "Error: file error." << std::endl;
			return 1;
		}
	}
	else // from stdin
	{
		int line_count = 1;
		std::cout << line_count++ << "> ";
		for (std::string line; std::getline(std::cin, line);)
		{
			ss << line << '\n';
			std::cout << line_count++ << "> ";
		}
	}

	input = ss.str();

	// interpreting
	try
	{
		Lexer lex(input);
		
		std::vector<Token> tokens = lex.GetTokens();

		Parser par(tokens);
		Interpreter interpreter;

		interpreter.Interpret(par.Parse());
	}
	catch (const Error& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}
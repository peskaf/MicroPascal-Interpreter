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
	if (argc != 2) // wrong usage
	{
		std::cout << "Usage: provide a name of the file that is to be interpreted as an argument." << std::endl;
		return 1;
	}
	else // read file
	{
		try
		{
			std::ifstream file(argv[1]);

			if (file.fail()) {
				throw std::exception();
			}

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
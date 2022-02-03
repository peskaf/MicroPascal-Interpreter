#include <iostream>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

int main(int argc, char const* argv[])
{
	try
	{
		/*
		std::string input =
			"Program test;\n"
			"ProCeDURE print(isProcedure:boolean, y:integer, a:char)\n"
			"{This procedure does something useful.}\n"
			"vAr in:string;\n"
			"begin\n"
			"	for i :=1 to 11 do\n"
			"		begin\n"
			"			writeln(i);\n"
			"		end;\n"
			"end;\n"
			"Var name:string;\n"
			"	 number:integer;\n"
			"\n"
			"beGin\n"
			"	nAme := 'Salvator'; { name of someone }\n"
			"	number := 1928;\n"
			"	writeln(name);\n"
			"	print(true, 12,		'f');\n"
			"enD.\n";
		*/
		std::string input = "1 + 1";

		Lexer lex(input);
		std::vector<Token> tokens = lex.GetTokens();

		Parser par(tokens);
		Interpreter interpreter;
		std::cout << std::get<int>(par.Parse()->Accept(interpreter)) << std::endl;

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
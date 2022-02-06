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

				"function p:integer;\n"
					"begin;\n"
					"p := 2;\n"
					"end;\n"

				"function f : integer;\n"
					"var j,z:boolean;\n"
					"l:string;\n"

					"function a : integer;\n"
						"var j,z:integer;\n"
						"l:string;\n"

						"function q : integer;\n"
						"var z : integer;\n"
						"begin\n"
							"z := 999; \n"
						"end;\n"

						"begin\n"
							"z := 44;\n"
							"a := p() + z + 88 + q();\n"
						"end;\n"

					"var t : boolean;\n"

				"begin\n"
					"f := 8 + a();\n"
				"end;\n"

			"begin		 \n"
				"i := f(); \n"
				"writeln(i);\n"
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
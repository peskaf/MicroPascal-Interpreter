#include <iostream>

int main(int argc, char const* argv[])
{
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
	return 0;
}
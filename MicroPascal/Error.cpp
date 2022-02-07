#include "Error.hpp"

Error::Error(int line, std::string message)
{
	error_message = ("[Line: " + std::to_string(line) + "] " + "Error: " + message);
}

std::string Error::what() const noexcept
{
	return error_message;
}
#include <stdexcept>
#include <sstream>

#include "Error.hpp"

void Error::ThrowError(int line, std::string message)
{
	std::stringstream ss;
	ss << "[Line: " << line << "] " << "Error: " << message;
	throw std::runtime_error(ss.str());
}
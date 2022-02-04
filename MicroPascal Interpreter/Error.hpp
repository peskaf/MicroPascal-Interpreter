#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

class Error
{
public:
	static void ThrowError(int line, std::string message);
};
#endif // !ERROR:HPP

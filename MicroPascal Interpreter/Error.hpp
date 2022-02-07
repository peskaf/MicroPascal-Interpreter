#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

class Error //: public std::exception
{
public:
	Error(int line, std::string message);
	std::string what() const noexcept;
private:
	std::string error_message;
};
#endif // !ERROR:HPP

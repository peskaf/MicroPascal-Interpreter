#ifndef CALLABLE_HPP
#define CALLABLE_HPP

#include <memory>

#include "Stmt.hpp"

class Callable
{
public:
	std::unique_ptr<Stmt> declaration;

};

#endif // !CALLABLE_HPP

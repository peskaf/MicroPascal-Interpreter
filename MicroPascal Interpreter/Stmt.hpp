#ifndef STMT_HPP
#define STMT_HPP

#include <optional>

#include "Token.hpp"
#include "Expr.hpp"

class WritelnStmt;

class VisitorStmt
{
public:
	virtual void Visit(WritelnStmt& writelnStmt) = 0;
};

class Stmt
{
public:
	// virtual ~Stmt();
	virtual void Accept(VisitorStmt& visitor) = 0;
};

class WritelnStmt : public Stmt
{
public:
	WritelnStmt(std::optional<std::unique_ptr<Expr>> m_expr);

	void Accept(VisitorStmt& visitor) override;

	std::optional<std::unique_ptr<Expr>> expr;
};


#endif STMT_HPP

#ifndef STMT_HPP
#define STMT_HPP

#include <optional>
#include <vector>

#include "Token.hpp"
#include "Expr.hpp"

class WritelnStmt;
class CompoundStmt;
class ProgramStmt;
class EmptyStmt;

class VisitorStmt
{
public:
	virtual void Visit(WritelnStmt& writelnStmt) = 0;
	virtual void Visit(CompoundStmt& compundStmt) = 0;
	virtual void Visit(ProgramStmt& programStmt) = 0;
	virtual void Visit(EmptyStmt& emptyStmt) = 0;
};

class Stmt
{
public:
	// virtual ~Stmt();
	virtual void Accept(VisitorStmt& visitor) = 0;
};

class ProgramStmt : public Stmt
{
public:
	ProgramStmt(std::unique_ptr<Stmt> m_stmt);

	void Accept(VisitorStmt& visitor) override;

	std::unique_ptr<Stmt> stmt;
};

class WritelnStmt : public Stmt
{
public:
	WritelnStmt(std::optional<std::vector<std::unique_ptr<Expr>>> m_exprs);

	void Accept(VisitorStmt& visitor) override;

	std::optional<std::vector<std::unique_ptr<Expr>>> exprs;
};

class CompoundStmt : public Stmt
{
public:
	CompoundStmt(std::vector<std::unique_ptr<Stmt>> m_statements);

	void Accept(VisitorStmt& visitor) override;

	std::vector<std::unique_ptr<Stmt>> statements;
};

class EmptyStmt : public Stmt
{
public:
	EmptyStmt();

	void Accept(VisitorStmt& visitor) override;
};



#endif STMT_HPP

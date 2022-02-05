#ifndef STMT_HPP
#define STMT_HPP

#include <optional>
#include <vector>
#include <unordered_map>

#include "Token.hpp"
#include "Expr.hpp"

class WritelnStmt;
class CompoundStmt;
class ProgramStmt;
class EmptyStmt;
class VarDeclStmt;

class VisitorStmt
{
public:
	virtual void Visit(WritelnStmt& writelnStmt) = 0;
	virtual void Visit(CompoundStmt& compundStmt) = 0;
	virtual void Visit(ProgramStmt& programStmt) = 0;
	virtual void Visit(EmptyStmt& emptyStmt) = 0;
	virtual void Visit(VarDeclStmt& varDeclStmt) = 0;
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
	ProgramStmt(std::string m_id, std::unique_ptr<Stmt> m_stmt, std::optional<std::vector<std::unique_ptr<Stmt>>> m_decl_stmts);

	void Accept(VisitorStmt& visitor) override;

	std::string id;
	std::optional<std::vector<std::unique_ptr<Stmt>>> decl_stmts;
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

class VarDeclStmt : public Stmt
{
public:
	VarDeclStmt(std::unordered_map<int, std::vector<Token>> m_variables);

	void Accept(VisitorStmt& visitor) override;

	std::unordered_map<int, std::vector<Token>> variables;
};



#endif // !STMT_HPP
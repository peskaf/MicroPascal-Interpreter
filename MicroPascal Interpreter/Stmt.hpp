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
class FuncDeclStmt;
class AssignmentStmt;
class IfStmt;
class WhileStmt;
class ForStmt;
class ProcDeclStmt;
class ProcedureCallStmt;

class VisitorStmt
{
public:
	virtual void Visit(WritelnStmt& writelnStmt) = 0;
	virtual void Visit(CompoundStmt& compundStmt) = 0;
	virtual void Visit(ProgramStmt& programStmt) = 0;
	virtual void Visit(EmptyStmt& emptyStmt) = 0;
	virtual void Visit(VarDeclStmt& varDeclStmt) = 0;
	virtual void Visit(FuncDeclStmt& funcDeclStmt) = 0;
	virtual void Visit(AssignmentStmt& assignmentStmt) = 0;
	virtual void Visit(IfStmt& ifStmt) = 0;
	virtual void Visit(WhileStmt& whileStmt) = 0;
	virtual void Visit(ForStmt& forStmt) = 0;
	virtual void Visit(ProcDeclStmt& procDeclStmt) = 0;
	virtual void Visit(ProcedureCallStmt& procedureCallStmt) = 0;
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
	ProgramStmt(std::string m_id, std::unique_ptr<Stmt> m_stmt, std::vector<std::shared_ptr<Stmt>> m_decl_stmts);

	void Accept(VisitorStmt& visitor) override;

	std::string id;
	std::vector<std::shared_ptr<Stmt>> decl_stmts;
	std::unique_ptr<Stmt> stmt;
};

class WritelnStmt : public Stmt
{
public:
	WritelnStmt(std::vector<std::unique_ptr<Expr>> m_exprs);

	void Accept(VisitorStmt& visitor) override;

	std::vector<std::unique_ptr<Expr>> exprs;
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
	VarDeclStmt(std::unordered_map<VariableType, std::vector<Token>> m_variables);

	void Accept(VisitorStmt& visitor) override;

	std::unordered_map<VariableType, std::vector<Token>> variables;
};

class FuncDeclStmt : public Stmt
{
public:
	FuncDeclStmt(Token m_id_token, VariableType m_return_type, std::shared_ptr<Stmt> m_body, std::vector<std::shared_ptr<Stmt>> m_decl_stmts, std::vector<std::pair<Token, VariableType>> m_parameters);

	void Accept(VisitorStmt& visitor) override;

	std::shared_ptr<Stmt> body;
	std::vector<std::shared_ptr<Stmt>> decl_stmts;
	Token id_token;
	VariableType return_type;
	std::vector<std::pair<Token, VariableType>> parameters;
};


class ProcDeclStmt : public Stmt
{
public:
	ProcDeclStmt(Token m_id_token, std::shared_ptr<Stmt> m_body, std::vector<std::shared_ptr<Stmt>> m_decl_stmts, std::vector<std::pair<Token, VariableType>> m_parameters);

	void Accept(VisitorStmt& visitor) override;

	std::shared_ptr<Stmt> body;
	std::vector<std::shared_ptr<Stmt>> decl_stmts;
	Token id_token;
	std::vector<std::pair<Token, VariableType>> parameters;
};

class ProcedureCallStmt : public Stmt
{
public:
	ProcedureCallStmt(std::vector<std::unique_ptr<Expr>> m_exprs, Token m_id_token);

	void Accept(VisitorStmt& visitor) override;

	Token id_token;
	std::vector<std::unique_ptr<Expr>> exprs; // arguments
};



class AssignmentStmt : public Stmt
{
public:
	AssignmentStmt(Token m_token, std::unique_ptr<Expr> m_value);

	void Accept(VisitorStmt& visitor) override;

	Token token;
	std::unique_ptr<Expr> value;
};

class IfStmt : public Stmt
{
public:
	IfStmt(Token m_token, std::unique_ptr<Expr> m_condition, std::unique_ptr<Stmt> m_then_branch, std::optional<std::unique_ptr<Stmt>> m_else_branch);

	void Accept(VisitorStmt& visitor) override;

	Token token;
	std::unique_ptr<Expr> condition;
	std::unique_ptr<Stmt> then_branch;
	std::optional<std::unique_ptr<Stmt>> else_branch;
};

class WhileStmt : public Stmt
{
public:
	WhileStmt(Token m_token, std::unique_ptr<Expr> m_condition, std::unique_ptr<Stmt> m_body);

	void Accept(VisitorStmt& visitor) override;

	Token token;
	std::unique_ptr<Expr> condition;
	std::unique_ptr<Stmt> body;
};

class ForStmt : public Stmt
{
public:
	ForStmt(Token m_for_token, bool m_increment, Token m_id_token, std::unique_ptr<Stmt> m_assignment, std::unique_ptr<Expr> m_expression, std::unique_ptr<Stmt> m_body);

	void Accept(VisitorStmt& visitor) override;

	Token for_token;
	bool increment;
	Token id_token;
	std::unique_ptr<Stmt> assignment;
	std::unique_ptr<Expr> expression;
	std::unique_ptr<Stmt> body;
};


#endif // !STMT_HPP
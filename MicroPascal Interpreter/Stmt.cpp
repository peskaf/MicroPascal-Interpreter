#include "Stmt.hpp"

ProgramStmt::ProgramStmt(std::string m_id, std::unique_ptr<Stmt> m_stmt, std::vector<std::unique_ptr<Stmt>> m_decl_stmts)
	: id(m_id), stmt(std::move(m_stmt)), decl_stmts(std::move(m_decl_stmts)) {};

void ProgramStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

CompoundStmt::CompoundStmt(std::vector<std::unique_ptr<Stmt>> m_statements) : statements(std::move(m_statements)) {};

void CompoundStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

WritelnStmt::WritelnStmt(std::vector<std::unique_ptr<Expr>> m_exprs) : exprs(std::move(m_exprs)) {};

void WritelnStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
};

EmptyStmt::EmptyStmt() {};

void EmptyStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

VarDeclStmt::VarDeclStmt(std::unordered_map<VariableType, std::vector<Token>> m_variables) : variables(m_variables) {};

void VarDeclStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

AssignmentStmt::AssignmentStmt(Token m_token, std::unique_ptr<Expr> m_value) : token(m_token), value(std::move(m_value)) {};

void AssignmentStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

IfStmt::IfStmt(Token m_token, std::unique_ptr<Expr> m_condition, std::unique_ptr<Stmt> m_then_branch, std::optional<std::unique_ptr<Stmt>> m_else_branch) :
	token(m_token), condition(std::move(m_condition)), then_branch(std::move(m_then_branch)), else_branch(std::move(m_else_branch)) {};

void IfStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

WhileStmt::WhileStmt(Token m_token, std::unique_ptr<Expr> m_condition, std::unique_ptr<Stmt> m_body) :
	token(m_token), condition(std::move(m_condition)), body(std::move(m_body)) {};

void WhileStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

ForStmt::ForStmt(Token m_for_token, bool m_increment, Token m_id_token, std::unique_ptr<Stmt> m_assignment, std::unique_ptr<Expr> m_expression, std::unique_ptr<Stmt> m_body) :
	for_token(m_for_token), increment(m_increment), id_token(m_id_token), assignment(std::move(m_assignment)), expression(std::move(m_expression)), body(std::move(m_body)) {}

void ForStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

FuncDeclStmt::FuncDeclStmt(Token m_id_token, VariableType m_return_type, std::shared_ptr<Stmt> m_body, std::vector<std::unique_ptr<Stmt>> m_decl_stmts, std::vector<std::pair<Token, VariableType>> m_parameters) : 
	id_token(m_id_token), return_type(m_return_type), body(std::move(m_body)), decl_stmts(std::move(m_decl_stmts)), parameters(m_parameters) {};

void FuncDeclStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

ProcDeclStmt::ProcDeclStmt(Token m_id_token, std::shared_ptr<Stmt> m_body, std::vector<std::unique_ptr<Stmt>> m_decl_stmts, std::vector<std::pair<Token, VariableType>> m_parameters) :
	id_token(m_id_token), body(std::move(m_body)), decl_stmts(std::move(m_decl_stmts)), parameters(m_parameters) {};

void ProcDeclStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

ProcedureCallStmt::ProcedureCallStmt(std::vector<std::unique_ptr<Expr>> m_exprs, Token m_id_token) : exprs(std::move(m_exprs)), id_token(m_id_token) {};


void ProcedureCallStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

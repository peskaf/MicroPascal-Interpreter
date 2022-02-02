#include <variant>
#include <string>
#include <memory>

#include "Token.hpp"

class Visitor
{
public:
	virtual Literal Visit(BinaryExpr& binExpr) = 0;
	virtual Literal Visit(UnaryExpr& unExpr) = 0;
	virtual Literal Visit(LiteralExpr& litExpr) = 0;
	virtual Literal Visit(GroupingExpr& grExpr) = 0;
};

// pridat reseni kompatibility typu a dalsi typy a vypisy erroru!!!!!!!!!!!!, rozdelit do separatnich souboru (interpreter, expr)
class Interpreter : public Visitor
{
public:
	Literal Visit(BinaryExpr& binExpr)
	{
		switch (binExpr.op.type)
		{
		case TokenType::PLUS:
			return std::get<int>(binExpr.left->Accept(*this)) + std::get<int>(binExpr.right->Accept(*this)); // for int only, assumes no error
		case TokenType::MINUS:
			return std::get<int>(binExpr.left->Accept(*this)) - std::get<int>(binExpr.right->Accept(*this)); // for int only, assumes no error
		case TokenType::MUL:
			return std::get<int>(binExpr.left->Accept(*this)) * std::get<int>(binExpr.right->Accept(*this)); // for int only, assumes no error
		case TokenType::DIV:
			return std::get<int>(binExpr.left->Accept(*this)) / std::get<int>(binExpr.right->Accept(*this)); // for int only, assumes no error
		default:
			// sem neco dat
			break;
		}
	}

	Literal Visit(LiteralExpr& litExpr)
	{
		return std::get<int>(litExpr.value);
	}

	Literal Visit(UnaryExpr& unExpr)
	{
		switch (unExpr.op.type)
		{
		case TokenType::MINUS:
			return - std::get<int>(unExpr.right->Accept(*this));
		default:
			break;
		}
	}

	Literal Visit(GroupingExpr& grExpr)
	{
		return grExpr.expr->Accept(*this);
	}
};

class Expr
{
public:
	virtual ~Expr() {};
	virtual Literal Accept(Visitor& visitor) = 0;
};

class BinaryExpr : public Expr
{
public:
	BinaryExpr(std::unique_ptr<Expr> m_left, std::unique_ptr<Expr> m_right, Token& m_op) : left(std::move(m_left)), right(std::move(m_right)), op(m_op) {};
	
	Literal Accept(Visitor& visitor) {}; // TODO: implement visitor -> returns visitor.visit(this)

	std::unique_ptr<Expr> left;
	std::unique_ptr<Expr> right;
	Token op;
};

class UnaryExpr : public Expr
{
public:
	UnaryExpr(std::unique_ptr<Expr> m_right, Token& m_op) : right(std::move(m_right)), op(m_op) {};

	Literal Accept(Visitor& visitor) {};

	std::unique_ptr<Expr> right;
	Token op;
};

class LiteralExpr : public Expr
{
public:
	LiteralExpr(Literal m_value) : value(m_value) {};

	Literal Accept(Visitor& visitor) {};

	Literal value;
};

class GroupingExpr : public Expr // ( ... ); keep??
{
public:
	GroupingExpr(std::unique_ptr<Expr> m_expr) : expr(std::move(m_expr)) {};

	Literal Accept(Visitor& visitor) {};

	std::unique_ptr<Expr> expr;
};
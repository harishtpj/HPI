#pragma once
#include "scanner/token.hpp"
class Expr;         // forward declare
class BinaryExpr;   // forward declare
class GroupingExpr; // forward declare
class LiteralExpr;  // forward declare
class UnaryExpr;    // forward declare
class ExprVisitor
{
public:
    virtual ~ExprVisitor() {}
    virtual std::any visitBinaryExpr(BinaryExpr *Expr) = 0;
    virtual std::any visitGroupingExpr(GroupingExpr *Expr) = 0;
    virtual std::any visitLiteralExpr(LiteralExpr *Expr) = 0;
    virtual std::any visitUnaryExpr(UnaryExpr *Expr) = 0;
};
class Expr
{
public:
    virtual ~Expr() {}
    virtual std::any accept(ExprVisitor *visitor) = 0;
};
class BinaryExpr : public Expr
{
public:
    BinaryExpr(Expr *left, Token Operator, Expr *right) : left(left), Operator(Operator), right(right) {}
    std::any accept(ExprVisitor *visitor) override
    {
        return visitor->visitBinaryExpr(this);
    }

public:
    Expr *left;
    Token Operator;
    Expr *right;
};
class GroupingExpr : public Expr
{
public:
    GroupingExpr(Expr *expression) : expression(expression) {}
    std::any accept(ExprVisitor *visitor) override
    {
        return visitor->visitGroupingExpr(this);
    }

public:
    Expr *expression;
};
class LiteralExpr : public Expr
{
public:
    LiteralExpr(std::any value) : value(value) {}
    std::any accept(ExprVisitor *visitor) override
    {
        return visitor->visitLiteralExpr(this);
    }

public:
    std::any value;
};
class UnaryExpr : public Expr
{
public:
    UnaryExpr(Token Operator, Expr *right) : Operator(Operator), right(right) {}
    std::any accept(ExprVisitor *visitor) override
    {
        return visitor->visitUnaryExpr(this);
    }

public:
    Token Operator;
    Expr *right;
};

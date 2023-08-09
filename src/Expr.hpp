#pragma once
#include "scanner/token.hpp"
class Expr; // forward declare
class BinaryExpr   ; // forward declare
class GroupingExpr ; // forward declare
class LiteralExpr  ; // forward declare
class UnaryExpr    ; // forward declare
class ExprVisitor {
public:
virtual ~ExprVisitor() {}
virtual void visitBinaryExpr   (BinaryExpr   * Expr) = 0;
virtual void visitGroupingExpr (GroupingExpr * Expr) = 0;
virtual void visitLiteralExpr  (LiteralExpr  * Expr) = 0;
virtual void visitUnaryExpr    (UnaryExpr    * Expr) = 0;
};
class Expr {
public:
virtual ~Expr() {}
virtual void accept(ExprVisitor* visitor) = 0;
};
class BinaryExpr    : public Expr { 
public: 
BinaryExpr   (Expr* left, Token Operator, Expr* right)  : left(left), Operator(Operator), right(right) {}
void accept(ExprVisitor* visitor) override {
visitor->visitBinaryExpr   (this);
}
public: 
Expr* left;
Token Operator;
Expr* right;
};
class GroupingExpr  : public Expr { 
public: 
GroupingExpr (Expr* expression)  : expression(expression) {}
void accept(ExprVisitor* visitor) override {
visitor->visitGroupingExpr (this);
}
public: 
Expr* expression;
};
class LiteralExpr   : public Expr { 
public: 
LiteralExpr  (std::string value)  : value(value) {}
void accept(ExprVisitor* visitor) override {
visitor->visitLiteralExpr  (this);
}
public: 
std::string value;
};
class UnaryExpr     : public Expr { 
public: 
UnaryExpr    (Token Operator, Expr* right)  : Operator(Operator), right(right) {}
void accept(ExprVisitor* visitor) override {
visitor->visitUnaryExpr    (this);
}
public: 
Token Operator;
Expr* right;
};

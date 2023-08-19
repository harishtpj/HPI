#pragma once
#include "scanner/token.hpp"
#include <vector>

class Expr; // forward declare
class AssignExpr   ; // forward declare
class BinaryExpr   ; // forward declare
class CallExpr     ; // forward declare
class GroupingExpr ; // forward declare
class LiteralExpr  ; // forward declare
class LogicalExpr  ; // forward declare
class UnaryExpr    ; // forward declare
class VariableExpr ; // forward declare
class ExprVisitor {
public:
    virtual ~ExprVisitor() {}
    virtual std::any visitAssignExpr   (AssignExpr   * Expr) = 0;
    virtual std::any visitBinaryExpr   (BinaryExpr   * Expr) = 0;
    virtual std::any visitCallExpr     (CallExpr     * Expr) = 0;
    virtual std::any visitGroupingExpr (GroupingExpr * Expr) = 0;
    virtual std::any visitLiteralExpr  (LiteralExpr  * Expr) = 0;
    virtual std::any visitLogicalExpr  (LogicalExpr  * Expr) = 0;
    virtual std::any visitUnaryExpr    (UnaryExpr    * Expr) = 0;
    virtual std::any visitVariableExpr (VariableExpr * Expr) = 0;
};
class Expr {
public:
    virtual ~Expr() {}
    virtual std::any accept(ExprVisitor* visitor) = 0;
};
class AssignExpr    : public Expr {
public:
    AssignExpr   (Token name, Expr* value)  : name(name), value(value) {}
    std::any accept(ExprVisitor* visitor) override {
        return visitor->visitAssignExpr   (this);
    }
public:
    Token name;
    Expr* value;
};
class BinaryExpr    : public Expr {
public:
    BinaryExpr   (Expr* left, Token Operator, Expr* right)  : left(left), Operator(Operator), right(right) {}
    std::any accept(ExprVisitor* visitor) override {
        return visitor->visitBinaryExpr   (this);
    }
public:
    Expr* left;
    Token Operator;
    Expr* right;
};
class CallExpr      : public Expr {
public:
    CallExpr     (Expr* callee, Token paren, std::vector<Expr*> args)  : callee(callee), paren(paren), args(args) {}
    std::any accept(ExprVisitor* visitor) override {
        return visitor->visitCallExpr     (this);
    }
public:
    Expr* callee;
    Token paren;
    std::vector<Expr*> args;
};
class GroupingExpr  : public Expr {
public:
    GroupingExpr (Expr* expression)  : expression(expression) {}
    std::any accept(ExprVisitor* visitor) override {
        return visitor->visitGroupingExpr (this);
    }
public:
    Expr* expression;
};
class LiteralExpr   : public Expr {
public:
    LiteralExpr  (std::any value)  : value(value) {}
    std::any accept(ExprVisitor* visitor) override {
        return visitor->visitLiteralExpr  (this);
    }
public:
    std::any value;
};
class LogicalExpr   : public Expr {
public:
    LogicalExpr  (Expr* left, Token Operator, Expr* right)  : left(left), Operator(Operator), right(right) {}
    std::any accept(ExprVisitor* visitor) override {
        return visitor->visitLogicalExpr  (this);
    }
public:
    Expr* left;
    Token Operator;
    Expr* right;
};
class UnaryExpr     : public Expr {
public:
    UnaryExpr    (Token Operator, Expr* right)  : Operator(Operator), right(right) {}
    std::any accept(ExprVisitor* visitor) override {
        return visitor->visitUnaryExpr    (this);
    }
public:
    Token Operator;
    Expr* right;
};
class VariableExpr  : public Expr {
public:
    VariableExpr (Token name)  : name(name) {}
    std::any accept(ExprVisitor* visitor) override {
        return visitor->visitVariableExpr (this);
    }
public:
    Token name;
};

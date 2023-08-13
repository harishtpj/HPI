#pragma once
#include "scanner/token.hpp"
#include "Expr.hpp"
class Stmt; // forward declare
class ExpressionStmt ; // forward declare
class PrintStmt      ; // forward declare
class StmtVisitor {
public:
    virtual ~StmtVisitor() {}
    virtual std::any visitExpressionStmt (ExpressionStmt * Stmt) = 0;
    virtual std::any visitPrintStmt      (PrintStmt      * Stmt) = 0;
};
class Stmt {
public:
    virtual ~Stmt() {}
    virtual std::any accept(StmtVisitor* visitor) = 0;
};
class ExpressionStmt  : public Stmt {
public:
    ExpressionStmt (Expr* expression)  : expression(expression) {}
    std::any accept(StmtVisitor* visitor) override {
        return visitor->visitExpressionStmt (this);
    }
public:
    Expr* expression;
};
class PrintStmt       : public Stmt {
public:
    PrintStmt      (Expr* expression)  : expression(expression) {}
    std::any accept(StmtVisitor* visitor) override {
        return visitor->visitPrintStmt      (this);
    }
public:
    Expr* expression;
};

#pragma once

#include "../Expr.hpp"
#include "../Stmt.hpp"
#include "runtime_err.hpp"
#include <vector>

class Interpreter : public ExprVisitor, StmtVisitor {
    public:
        void interpret(vector<Stmt*> expression);
        string interpret(Expr* expr);
        any visitLiteralExpr(LiteralExpr* expr);
        any visitGroupingExpr(GroupingExpr* expr);
        any visitUnaryExpr(UnaryExpr* expr);
        any visitBinaryExpr(BinaryExpr* expr);

        any visitExpressionStmt(ExpressionStmt* stmt);
        any visitPrintStmt(PrintStmt* stmt);
    
    private:
        any evaluate(Expr* expr);
        any execute(Stmt* stmt);
        bool isTruthy(any value);
        bool isEqual(any left, any right);
        void checkNumberOperand(Token op, any operand);
        void checkNumberOperands(Token op, any left, any right);
        string stringify(any value);
};

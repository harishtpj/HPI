#pragma once

#include "../Expr.hpp"
#include "runtime_err.hpp"

class Interpreter : public ExprVisitor {
    public:
        void interpret(Expr* expression);
        any visitLiteralExpr(LiteralExpr* expr);
        any visitGroupingExpr(GroupingExpr* expr);
        any visitUnaryExpr(UnaryExpr* expr);
        any visitBinaryExpr(BinaryExpr* expr);
    
    private:
        any evaluate(Expr* expr);
        bool isTruthy(any value);
        bool isEqual(any left, any right);
        void checkNumberOperand(Token op, any operand);
        void checkNumberOperands(Token op, any left, any right);
        string stringify(any value);
};

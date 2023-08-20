#pragma once

#include "../Expr.hpp"
#include "../Stmt.hpp"
#include "runtime_err.hpp"
#include <vector>
#include "environment.hpp"
#include "HPICallable.hpp"
#include "HPIFunction.hpp"

class BreakException: public runtime_error {
    public:
        BreakException(): runtime_error("break") {}
};

class ReturnException: public runtime_error {
    public:
        any value;
        ReturnException(any value): runtime_error("ret"), value(value) {}
};

class Interpreter : public ExprVisitor, StmtVisitor {
    public:
        Environment* globals = new Environment();
        
        Interpreter();
        void interpret(vector<Stmt*> expression);
        string interpret(Expr* expr);
        any visitLogicalExpr(LogicalExpr* expr);
        any visitLiteralExpr(LiteralExpr* expr);
        any visitGroupingExpr(GroupingExpr* expr);
        any visitUnaryExpr(UnaryExpr* expr);
        any visitBinaryExpr(BinaryExpr* expr);
        any visitExpressionStmt(ExpressionStmt* stmt);
        any visitPrintStmt(PrintStmt* stmt);
        any visitBlockStmt(BlockStmt* stmt);
        any visitIfStmt(IfStmt* stmt);
        any visitLoopStmt(LoopStmt* stmt);
        any visitBreakStmt(BreakStmt* stmt);
        any visitReturnStmt(ReturnStmt* stmt);
        any visitVarStmt(VarStmt* stmt);
        any visitVariableExpr(VariableExpr* expr);
        any visitAssignExpr(AssignExpr* expr);
        any visitFunctionExpr(FunctionExpr* expr);
        any visitCallExpr(CallExpr* expr);
        any visitFunctionStmt(FunctionStmt* stmt);

        void executeBlock(vector<Stmt*> stmts, Environment* env);
        string stringify(any value);
    
    private:
        Environment* environment = globals;

        any evaluate(Expr* expr);
        any execute(Stmt* stmt);
        bool isTruthy(any value);
        bool isEqual(any left, any right);
        void checkNumberOperand(Token op, any operand);
        void checkNumberOperands(Token op, any left, any right);
};

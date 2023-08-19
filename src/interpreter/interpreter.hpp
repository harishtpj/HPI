#pragma once

#include "../Expr.hpp"
#include "../Stmt.hpp"
#include "runtime_err.hpp"
#include <vector>
#include "environment.hpp"
#include "HPICallable.hpp"

class BreakException: public runtime_error {
    public:
        BreakException(): runtime_error("break") {}
};

class Interpreter : public ExprVisitor, StmtVisitor {
    public:
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
        any visitVarStmt(VarStmt* stmt);
        any visitVariableExpr(VariableExpr* expr);
        any visitAssignExpr(AssignExpr* expr);
        any visitCallExpr(CallExpr* expr);
    
    private:
        Environment globals;
        Environment environment;

        any evaluate(Expr* expr);
        any execute(Stmt* stmt);
        bool isTruthy(any value);
        bool isEqual(any left, any right);
        void checkNumberOperand(Token op, any operand);
        void checkNumberOperands(Token op, any left, any right);
        string stringify(any value);
        void executeBlock(vector<Stmt*> stmts, Environment env);
};

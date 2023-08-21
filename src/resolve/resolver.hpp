#pragma once

#include <vector>
#include <map>
#include "../Stmt.hpp"
#include "../interpreter/interpreter.hpp"

enum class FunctionType {
    NONE,
    FUNCTION
};

class Resolver: public ExprVisitor, public StmtVisitor {
    public:
        Resolver(Interpreter& interpreter);

        void resolve(std::vector<Stmt*> statements);

        any visitBlockStmt(BlockStmt* stmt);
        any visitVarStmt(VarStmt* stmt);
        any visitFunctionStmt(FunctionStmt* stmt);
        any visitExpressionStmt(ExpressionStmt* stmt);
        any visitIfStmt(IfStmt* stmt);
        any visitPrintStmt(PrintStmt* stmt);
        any visitReturnStmt(ReturnStmt* stmt);
        any visitLoopStmt(LoopStmt* stmt);
        any visitBreakStmt(BreakStmt* stmt);

        any visitVariableExpr(VariableExpr* expr);
        any visitAssignExpr(AssignExpr* expr);
        any visitBinaryExpr(BinaryExpr* expr);
        any visitCallExpr(CallExpr* expr);
        any visitGroupingExpr(GroupingExpr* expr);
        any visitLiteralExpr(LiteralExpr* expr);
        any visitLogicalExpr(LogicalExpr* expr);
        any visitUnaryExpr(UnaryExpr* expr);
        any visitFunctionExpr(FunctionExpr* expr);

    private:
        Interpreter& interpreter;
        vector<map<string, bool>> scopes;
        FunctionType currentFunction = FunctionType::NONE;

        void resolve(Stmt* stmt);
        void resolve(Expr* expr);
        void resolveLocal(Expr* expr, Token name);
        void resolveFunction(FunctionExpr* fn, FunctionType type);
        void beginScope();
        void endScope();
        void declare(Token name);
        void define(Token name);
};
#include "resolver.hpp"
#include "../hpi.hpp"

Resolver::Resolver(Interpreter& interpreter) : interpreter(interpreter) {}

void Resolver::resolve(vector<Stmt*> statements) {
    for (Stmt* stmt: statements) {
        resolve(stmt);
    }
}

any Resolver::visitBlockStmt(BlockStmt* stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
    return nullptr;
}

any Resolver::visitVarStmt(VarStmt* stmt) {
    declare(stmt->name);
    if (stmt->initializer != nullptr) {
        resolve(stmt->initializer);
    }
    define(stmt->name);
    return nullptr;
}

any Resolver::visitFunctionStmt(FunctionStmt* stmt) {
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(stmt->fn, FunctionType::FUNCTION);
    return nullptr;
}

any Resolver::visitExpressionStmt(ExpressionStmt* stmt) {
    resolve(stmt->expression);
    return nullptr;
}

any Resolver::visitIfStmt(IfStmt* stmt) {
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch != nullptr) resolve(stmt->elseBranch);
    return nullptr;
}

any Resolver::visitPrintStmt(PrintStmt* stmt) {
    resolve(stmt->expression);
    return nullptr;
}

any Resolver::visitReturnStmt(ReturnStmt* stmt) {
    if (currentFunction == FunctionType::NONE) {
        HPI::error(stmt->keyword, "Can't return from top-level code.");
    }

    if (stmt->value != nullptr) {
        resolve(stmt->value);
    }
    return nullptr;
}

any Resolver::visitLoopStmt(LoopStmt* stmt) {
    resolve(stmt->body);
    return nullptr;
}

any Resolver::visitBreakStmt(BreakStmt* stmt) {
    return nullptr;
}

any Resolver::visitVariableExpr(VariableExpr* expr) {
    if (!scopes.empty()) {
        auto& currentScope = scopes.back();
        auto elem = currentScope.find(expr->name.lexeme);
        if (elem != currentScope.end() && elem->second == false) {
            HPI::error(expr->name,
                      "Can't read local variable in its own initializer.");
        }
    }
    resolveLocal(expr, expr->name);
    return nullptr;
}

any Resolver::visitAssignExpr(AssignExpr* expr) {
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return nullptr;
}

any Resolver::visitBinaryExpr(BinaryExpr* expr) {
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}

any Resolver::visitCallExpr(CallExpr* expr) {
    resolve(expr->callee);
    for (Expr* argument: expr->args) {
        resolve(argument);
    }
    return nullptr;
}

any Resolver::visitGroupingExpr(GroupingExpr* expr) {
    resolve(expr->expression);
    return nullptr;
}

any Resolver::visitLiteralExpr(LiteralExpr* expr) {
    return nullptr;
}

any Resolver::visitLogicalExpr(LogicalExpr* expr) {
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}

any Resolver::visitUnaryExpr(UnaryExpr* expr) {
    resolve(expr->right);
    return nullptr;
}

any Resolver::visitFunctionExpr(FunctionExpr* expr) {
    resolveFunction(expr, FunctionType::FUNCTION);
    return nullptr;
}

void Resolver::resolve(Stmt* stmt) {
    stmt->accept(this);
}

void Resolver::resolve(Expr* expr) {
    expr->accept(this);
}

void Resolver::resolveLocal(Expr* expr, Token name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        if (scopes[i].find(name.lexeme) != scopes[i].end()) {
            interpreter.resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}

void Resolver::resolveFunction(FunctionExpr* fn, FunctionType type) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (Token param: fn->params) {
        declare(param);
        define(param);
    }
    resolve(fn->body);
    endScope();
    currentFunction = enclosingFunction;
}

void Resolver::beginScope() {
    scopes.push_back(map<string, bool>{});
}

void Resolver::endScope() {
    scopes.pop_back();
}

void Resolver::declare(Token name) {
    if (scopes.empty()) return;

    map<string, bool>& scope = scopes.back();
    if (scope.find(name.lexeme) != scope.end()) {
        HPI::error(name, "Already a variable with this name in this scope.");
    }
    scope[name.lexeme] = false;
}

void Resolver::define(Token name) {
    if (scopes.empty()) return;
    scopes.back()[name.lexeme] = true;
}
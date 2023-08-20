#pragma once
#include "scanner/token.hpp"
#include <vector>
#include "Expr.hpp"
class Stmt; // forward declare
class BlockStmt      ; // forward declare
class BreakStmt      ; // forward declare
class ExpressionStmt ; // forward declare
class FunctionStmt   ; // forward declare
class IfStmt         ; // forward declare
class PrintStmt      ; // forward declare
class ReturnStmt     ; // forward declare
class VarStmt        ; // forward declare
class LoopStmt       ; // forward declare
class StmtVisitor {
public:
    virtual ~StmtVisitor() {}
    virtual std::any visitBlockStmt      (BlockStmt      * Stmt) = 0;
    virtual std::any visitBreakStmt      (BreakStmt      * Stmt) = 0;
    virtual std::any visitExpressionStmt (ExpressionStmt * Stmt) = 0;
    virtual std::any visitFunctionStmt   (FunctionStmt   * Stmt) = 0;
    virtual std::any visitIfStmt         (IfStmt         * Stmt) = 0;
    virtual std::any visitPrintStmt      (PrintStmt      * Stmt) = 0;
    virtual std::any visitReturnStmt     (ReturnStmt     * Stmt) = 0;
    virtual std::any visitVarStmt        (VarStmt        * Stmt) = 0;
    virtual std::any visitLoopStmt       (LoopStmt       * Stmt) = 0;
};
class Stmt {
public:
    virtual ~Stmt() {}
    virtual std::any accept(StmtVisitor* visitor) = 0;
};
class BlockStmt       : public Stmt {
public:
    BlockStmt      (std::vector<Stmt*> statements)  : statements(statements) {}
    std::any accept(StmtVisitor* visitor) override {
        return visitor->visitBlockStmt      (this);
    }
public:
    std::vector<Stmt*> statements;
};
class BreakStmt       : public Stmt {
public:
    BreakStmt      ()  {}
    std::any accept(StmtVisitor* visitor) override {
        return visitor->visitBreakStmt      (this);
    }
public:
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
class FunctionStmt    : public Stmt {
public:
    FunctionStmt   (Token name, std::vector<Token> params, std::vector<Stmt*> body)  : name(name), params(params), body(body) {}
    std::any accept(StmtVisitor* visitor) override {
        return visitor->visitFunctionStmt   (this);
    }
public:
    Token name;
    std::vector<Token> params;
    std::vector<Stmt*> body;
};
class IfStmt          : public Stmt {
public:
    IfStmt         (Expr* condition, Stmt* thenBranch, Stmt* elseBranch)  : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
    std::any accept(StmtVisitor* visitor) override {
        return visitor->visitIfStmt         (this);
    }
public:
    Expr* condition;
    Stmt* thenBranch;
    Stmt* elseBranch;
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
class ReturnStmt      : public Stmt {
public:
    ReturnStmt     (Token keyword, Expr* value)  : keyword(keyword), value(value) {}
    std::any accept(StmtVisitor* visitor) override {
        return visitor->visitReturnStmt     (this);
    }
public:
    Token keyword;
    Expr* value;
};
class VarStmt         : public Stmt {
public:
    VarStmt        (Token name, Expr* initializer)  : name(name), initializer(initializer) {}
    std::any accept(StmtVisitor* visitor) override {
        return visitor->visitVarStmt        (this);
    }
public:
    Token name;
    Expr* initializer;
};
class LoopStmt        : public Stmt {
public:
    LoopStmt       (Stmt* body)  : body(body) {}
    std::any accept(StmtVisitor* visitor) override {
        return visitor->visitLoopStmt       (this);
    }
public:
    Stmt* body;
};

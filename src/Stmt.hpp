#pragma once
#include "scanner/token.hpp"
#include "Expr.hpp"
#include <vector>
class Stmt; // forward declare
class BlockStmt      ; // forward declare
class BreakStmt      ; // forward declare
class ExpressionStmt ; // forward declare
class IfStmt         ; // forward declare
class PrintStmt      ; // forward declare
class VarStmt        ; // forward declare
class RepeatStmt       ; // forward declare
class StmtVisitor {
public:
    virtual ~StmtVisitor() {}
    virtual std::any visitBlockStmt      (BlockStmt      * Stmt) = 0;
    virtual std::any visitBreakStmt      (BreakStmt      * Stmt) = 0;
    virtual std::any visitExpressionStmt (ExpressionStmt * Stmt) = 0;
    virtual std::any visitIfStmt         (IfStmt         * Stmt) = 0;
    virtual std::any visitPrintStmt      (PrintStmt      * Stmt) = 0;
    virtual std::any visitVarStmt        (VarStmt        * Stmt) = 0;
    virtual std::any visitRepeatStmt       (RepeatStmt       * Stmt) = 0;
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
class RepeatStmt        : public Stmt {
public:
    RepeatStmt       (Stmt* body)  : body(body) {}
    std::any accept(StmtVisitor* visitor) override {
        return visitor->visitRepeatStmt       (this);
    }
public:
    Stmt* body;
};

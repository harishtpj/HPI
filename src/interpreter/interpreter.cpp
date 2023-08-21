#include "interpreter.hpp"
#include "builtin.hpp"
#include "../hpi.hpp"
#include <iostream>

Interpreter::Interpreter() {
    // Native Functions
    globals->define("clock", new Clock());
    globals->define("read", new Read());
    globals->define("fmt", new Fmt());
}

void Interpreter::interpret(vector<Stmt*> stmts) {
    try {
        for (Stmt* &stmt: stmts) {
            execute(stmt);
        }
    } catch(const RuntimeError& e) {
        HPI::runtimeError(e);
    }
}

string Interpreter::interpret(Expr* expr) {
    try {
        any value = evaluate(expr);
        return stringify(value);
    } catch(const RuntimeError& e) {
        HPI::runtimeError(e);
        return "expr null";
    }
}

any Interpreter::visitAssignExpr(AssignExpr* expr) {
    any value = evaluate(expr->value);

    auto elem = locals.find(expr);
    if (elem != locals.end()) {
        int distance = elem->second;
        environment->assignAt(distance, expr->name, value);
    } else {
        globals->assign(expr->name, value);
    }

    return value;
}

any Interpreter::visitLogicalExpr(LogicalExpr* expr) {
    any left = evaluate(expr->left);

    if ((expr->Operator).type == TokenType::OR) {
        if (isTruthy(left)) return left;
    } else {
        if (!isTruthy(left)) return left;
    }

    return evaluate(expr->right);
}

any Interpreter::visitLiteralExpr(LiteralExpr* expr) {
    return expr->value;
}

any Interpreter::visitGroupingExpr(GroupingExpr* expr) {
    return evaluate(expr->expression);
}

any Interpreter::visitUnaryExpr(UnaryExpr* expr) {
    any right = evaluate(expr->right);

    switch (expr->Operator.type) {
        case TokenType::BANG:
            return !isTruthy(right);

        case TokenType::MINUS:
            checkNumberOperand(expr->Operator, right);
            return -any_cast<double>(right);
    }

    return nullptr;
}

any Interpreter::visitBinaryExpr(BinaryExpr* expr) {
    any left = evaluate(expr->left);
    any right = evaluate(expr->right);

    switch (expr->Operator.type) {
        case TokenType::BANG_EQUAL: 
            return !isEqual(left, right);

        case TokenType::EQUAL_EQUAL: 
            return isEqual(left, right);

        case TokenType::GREATER:
            checkNumberOperands(expr->Operator, left, right);
            return any_cast<double>(left) > any_cast<double>(right);
        
        case TokenType::GREATER_EQUAL:
            checkNumberOperands(expr->Operator, left, right);
            return any_cast<double>(left) >= any_cast<double>(right);

        case TokenType::LESS:
            checkNumberOperands(expr->Operator, left, right);
            return any_cast<double>(left) < any_cast<double>(right);

        case TokenType::LESS_EQUAL:
            checkNumberOperands(expr->Operator, left, right);
            return any_cast<double>(left) <= any_cast<double>(right);

        case TokenType::MINUS:
            checkNumberOperands(expr->Operator, left, right);
            return any_cast<double>(left) - any_cast<double>(right);

        case TokenType::PLUS:
            if (left.type() == typeid(double) && right.type() == typeid(double)) {
                return any_cast<double>(left) + any_cast<double>(right);
            }

            if (left.type() == typeid(string) && right.type() == typeid(string)) {
                return any_cast<string>(left) + any_cast<string>(right);
            }
            
            throw RuntimeError(expr->Operator, "Operands must be two numbers or two strings");

        case TokenType::SLASH:
            checkNumberOperands(expr->Operator, left, right);
            return any_cast<double>(left) / any_cast<double>(right);

        case TokenType::STAR:
            checkNumberOperands(expr->Operator, left, right);
            return any_cast<double>(left) * any_cast<double>(right);
    }

    return nullptr;
}

any Interpreter::visitCallExpr(CallExpr* expr) {
    auto callee = evaluate(expr->callee);
    vector<any> arguments;

    for (Expr* argument: expr->args) {
        arguments.push_back(evaluate(argument));
    }

    HPICallable* fn;
    if (callee.type() == typeid(HPIFunction*)) {
        fn = dynamic_cast<HPIFunction*>(any_cast<HPIFunction*>(callee));
    } else if (callee.type() == typeid(Clock*)) {
        fn = dynamic_cast<Clock*>(any_cast<Clock*>(callee));
    } else if (callee.type() == typeid(Read*)) {
        fn = dynamic_cast<Read*>(any_cast<Read*>(callee));
    } else if (callee.type() == typeid(Fmt*)) {
        Fmt* func = any_cast<Fmt*>(callee);
        func->paramCount = arguments.size();
        fn = dynamic_cast<Fmt*>(func);
    } else {
        throw RuntimeError(expr->paren, "Can only call functions.");
    }

    if (arguments.size() != fn->arity()) {
        throw RuntimeError(expr->paren, "Expected " + to_string(fn->arity()) + 
                            " arguments but got " + to_string(arguments.size()));
    }

    return fn->call(*this, arguments);
}

any Interpreter::visitExpressionStmt(ExpressionStmt* stmt) {
    evaluate(stmt->expression);
    return nullptr;
}

any Interpreter::visitPrintStmt(PrintStmt* stmt) {
    any value = evaluate(stmt->expression);
    cout << stringify(value);
    return nullptr;
}

any Interpreter::visitBlockStmt(BlockStmt* stmt) {
    executeBlock(stmt->statements, new Environment(environment));
    return nullptr;
}

any Interpreter::visitIfStmt(IfStmt* stmt) {
    if (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->thenBranch);
    } else if (stmt->elseBranch != nullptr) {
        execute(stmt->elseBranch);
    }
    return nullptr;
}

any Interpreter::visitLoopStmt(LoopStmt* stmt) {
    try {
        while (true) {
            execute(stmt->body);
        }
    } catch (const BreakException&) {
        // PASS
    }
    return nullptr;
}

any Interpreter::visitBreakStmt(BreakStmt* stmt) {
    throw BreakException();
}

any Interpreter::visitReturnStmt(ReturnStmt* stmt) {
    any value = nullptr;
    if (stmt->value != nullptr) {
        value = evaluate(stmt->value);
    }
    throw ReturnException(value);
}

any Interpreter::visitFunctionStmt(FunctionStmt* stmt) {
    string fnName = stmt->name.lexeme;
    HPIFunction* fn = new HPIFunction(fnName, stmt->fn, environment);
    environment->define(stmt->name.lexeme, fn);
    return nullptr;
}

any Interpreter::visitVarStmt(VarStmt* stmt) {
    any value = nullptr;

    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer);
    }
        
    environment->define(stmt->name.lexeme, value);
    return nullptr;
}

any Interpreter::visitVariableExpr(VariableExpr* expr) {
    return lookUpVariable(expr->name, expr);
}

any Interpreter::lookUpVariable(Token name, Expr* expr) {
    auto elem = locals.find(expr);
    if (elem != locals.end()) {
        int distance = elem->second;
        return environment->getAt(distance, name.lexeme);
    } else {
        return globals->get(name);
    }
}

any Interpreter::visitFunctionExpr(FunctionExpr* expr) {
    return new HPIFunction("", expr, environment);
}

any Interpreter::evaluate(Expr* expr) {
    return expr->accept(this);
}

any Interpreter::execute(Stmt* stmt) {
    stmt->accept(this);
    return nullptr;
}

void Interpreter::executeBlock(vector<Stmt*> stmts, Environment* env) {
    Environment* previous = this->environment;
    try {
        this->environment = env;

        for (Stmt* &stmt: stmts) {
            execute(stmt);
        }
    } catch (...) {
        this->environment = previous;
        throw;
    }
    this->environment = previous;
}

bool Interpreter::isTruthy(any object) {
    if (object.type() == typeid(nullptr)) return false;
    if (object.type() == typeid(bool)) {
        return any_cast<bool>(object);
    }
    return true;
}

bool Interpreter::isEqual(any left, any right) {
    if (left.type() == typeid(nullptr) && right.type() == typeid(nullptr)) {
        return true;
    }

    if (left.type() == typeid(nullptr)) {
        return false;
    }

    if (left.type() == typeid(double) && right.type() == typeid(double)) {
        return any_cast<double>(left) == any_cast<double>(right);
    }

    if (left.type() == typeid(string) && right.type() == typeid(string)) {
        return any_cast<string>(left) == any_cast<string>(right);
    }

    if (left.type() == typeid(bool) && right.type() == typeid(bool)) {
        return any_cast<bool>(left) == any_cast<bool>(right);
    }

    return false;
}

void Interpreter::checkNumberOperand(Token op, any operand) {
    if (operand.type() == typeid(double)) return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token op, any left, any right) {
    if (left.type() == typeid(double) && right.type() == typeid(double)) return;
    throw RuntimeError(op, "Operands must be numbers.");
}

string Interpreter::stringify(any object) {
    if (object.type() == typeid(nullptr)) return "nil";

    if (object.type() == typeid(double)) {
        string text =to_string(any_cast<double>(object));
        if (text[text.length() - 7] == '.' && text[text.length() - 6] == '0') {
            text = text.substr(0, text.length() - 7);
        }
        return text;
    }

    if (object.type() == typeid(string)) {
        return any_cast<std::string>(object);
    }

    if (object.type() == typeid(bool)) {
        return any_cast<bool>(object) ? "true" : "false";
    }

    if (object.type() == typeid(HPIFunction*)) {
        return any_cast<HPIFunction*>(object)->toString();
    }

    if (object.type() == typeid(Clock*)) {
        return any_cast<Clock*>(object)->toString();
    }

    if (object.type() == typeid(Read*)) {
        return any_cast<Read*>(object)->toString();
    }

    if (object.type() == typeid(Fmt*)) {
        return any_cast<Fmt*>(object)->toString();
    }

    return "stringify: cannot recognize type";
}

void Interpreter::resolve(Expr* expr, int depth) {
    locals[expr] = depth;
}
#include "interpreter.hpp"
#include "../hpi.hpp"
#include <iostream>

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

any Interpreter::visitExpressionStmt(ExpressionStmt* stmt) {
    evaluate(stmt->expression);
    return nullptr;
}

any Interpreter::visitPrintStmt(PrintStmt* stmt) {
    any value = evaluate(stmt->expression);
    cout << stringify(value);
    return nullptr;
}

any Interpreter::evaluate(Expr* expr) {
    return expr->accept(this);
}

any Interpreter::execute(Stmt* stmt) {
    stmt->accept(this);
    return nullptr;
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
    return "stringify: cannot recognize type";
}
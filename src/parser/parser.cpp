#include "parser.hpp"
#include <iostream>

ParseError::ParseError(string msg, Token token): runtime_error(msg), token(token) {}

Parser::Parser(const vector<Token>& tokens): tokens(tokens) {}

Stmt* Parser::declaration() {
    try {
        if (match({TokenType::LET})) return varDeclaration();
        
        return statement();
    } catch (const ParseError& e) {
        synchronize();
        return nullptr;
    }
}

Stmt* Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    Expr* initializer = nullptr;
    if (match({TokenType::EQUAL})) initializer = expression();

    if (!isRepl) consumeNewline();
    return new VarStmt(name, initializer);
}

Stmt* Parser::statement() {
    while (check(TokenType::NEWLINE)) advance();
    if (match({TokenType::PRINT})) return printStatement();
    if (match({TokenType::PRINTLN})) return printLnStatement();
    if (match({TokenType::DO})) return new BlockStmt(block());

    return expressionStatement();
}

Stmt* Parser::printStatement() {
    Expr* value = expression();
    if (!isRepl) consumeNewline();
    return new PrintStmt(value);
}

Stmt* Parser::printLnStatement() {
    Expr* value = expression();
    if (!isRepl) consumeNewline();

    Stmt* print = new PrintStmt(value);
    string nl{"\n"};
    Stmt* newLine = new PrintStmt(new LiteralExpr(nl));

    return new BlockStmt({print, newLine});
}

Stmt* Parser::expressionStatement() {
    Expr* expr = expression();

    if (isRepl && isAtEnd()) {
        foundExpression = true;
    } else {
        consumeNewline();
    }

    return new ExpressionStmt(expr);
}

vector<Stmt*> Parser::block() {
    vector<Stmt*> statements;
    while (check(TokenType::NEWLINE)) advance();

    while (!check(TokenType::END) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::END, "Expect 'end' after block.");
    return statements;
}

Expr* Parser::expression() {
    return assignment();
}

Expr* Parser::assignment() {
    Expr* expr = equality();

    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        Expr* value = assignment();

        if (VariableExpr* e = dynamic_cast<VariableExpr*>(expr)) {
            Token name = e->name;
            return new AssignExpr(name, value);
        }

        error(equals, "Invalid assignment target.");
    }

    return expr;
}

Expr* Parser::equality() {
    Expr* expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        Expr* right = comparison();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr* Parser::comparison() {
    Expr* expr = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, 
                        TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        Expr* right = term();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr* Parser::term() {
    Expr* expr = factor();

    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        Expr* right = factor();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr* Parser::factor() {
    Expr* expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        Expr* right = unary();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr* Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        Expr* right = unary();
        return new UnaryExpr(op, right);
    }

    return primary();
}

Expr* Parser::primary() {
    if (match({TokenType::FALSE})) return new LiteralExpr(false);
    if (match({TokenType::TRUE})) return new LiteralExpr(true);
    if (match({TokenType::NIL})) return new LiteralExpr(nullptr);

    if (match({TokenType::NUMBER, TokenType::STRING})) 
        return new LiteralExpr(previous().literal);
    
    if (match({TokenType::IDENTIFIER})) 
        return new VariableExpr(previous());

    if (match({TokenType::LEFT_PAREN})) {
        Expr* expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return new GroupingExpr(expr);
    }

    throw error(peek(), "Expect expression.");
}

vector<Stmt*> Parser::parse() {
    vector<Stmt*> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}

any Parser::parseRepl() {
    isRepl = true;
    vector<Stmt*> statements;

    while (!isAtEnd()) {
        statements.push_back(declaration());
        if (foundExpression) {
            Stmt* last = statements.back();
            return dynamic_cast<ExpressionStmt*>(last)->expression;
        }

        isRepl = false;
    }
    
    return statements;
}

bool Parser::match(const vector<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::E_O_F;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

Token Parser::consume(TokenType type, string message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

void Parser::consumeNewline() {
    if (isAtEnd()) return;
    consume(TokenType::NEWLINE, "Expect newline.");
    while (check(TokenType::NEWLINE)) advance();
}

ParseError Parser::error(Token token, string message) {
    HPI::error(token, message);
    return *new ParseError(message, token);
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        switch (peek().type) {
            case TokenType::FUNCTION:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
        }

        advance();
    }
}

#pragma once

#include <vector>

#include "../Expr.hpp"
#include "../Stmt.hpp"
#include "../hpi.hpp"

using namespace std;

class ParseError : public runtime_error {
    public:
        Token token;
        ParseError(string msg, Token token);
};

class Parser {
    public:
        Parser(const vector<Token>& tokens);
        vector<Stmt*> parse();
        any parseRepl();
    
    private:
        const vector<Token> tokens;
        bool isRepl = false;
        bool foundExpression = false;
        int current = 0;

        Stmt* declaration();
        Stmt* statement();
        Stmt* printStatement();
        Stmt* printLnStatement();
        Stmt* expressionStatement();
        vector<Stmt*> block();

        Stmt* varDeclaration();

        Expr* expression();
        Expr* assignment();
        Expr* equality();
        Expr* comparison();
        Expr* term();
        Expr* factor();
        Expr* unary();
        Expr* primary();

        bool match(const vector<TokenType>& types);
        bool check(TokenType type);
        Token advance();
        bool isAtEnd();
        Token peek();
        Token previous();
        Token consume(TokenType type, string message);
        void consumeNewline();
        ParseError error(Token token, string message);
        void synchronize();
};

#include <format>
#include "scanner.hpp"
#include "../hpi.hpp"
#include <iostream>
#include <algorithm>

Scanner::Scanner(string src): src(src) {
    keywords["and"] = TokenType::AND;
    keywords["else"] = TokenType::ELSE;
    keywords["false"] = TokenType::FALSE;
    keywords["for"] = TokenType::FOR;
    keywords["function"] = TokenType::FUNCTION;
    keywords["if"] = TokenType::IF;
    keywords["nil"] = TokenType::NIL;
    keywords["or"] = TokenType::OR;
    keywords["print"] = TokenType::PRINT;
    keywords["println"] = TokenType::PRINTLN;
    keywords["input"] = TokenType::INPUT;
    keywords["return"] = TokenType::RETURN;
    keywords["true"] = TokenType::TRUE;
    keywords["while"] = TokenType::WHILE;
    keywords["do"] = TokenType::DO;
    keywords["end"] = TokenType::END;
    keywords["to"] = TokenType::TO;
    keywords["then"] = TokenType::THEN;
    keywords["let"] = TokenType::LET;
    keywords["break"] = TokenType::BREAK;
    keywords["loop"] = TokenType::LOOP;
    keywords["repeat"] = TokenType::REPEAT;

    // Support uppercase statements
    for (auto &[key, value] : keywords) {
        string upperKeyword = key;
        transform(upperKeyword.begin(), upperKeyword.end(), upperKeyword.begin(), ::toupper);
        keywords[upperKeyword] = value;
    }
    
}

vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    
    tokens.push_back(Token(TokenType::E_O_F, "", "", line));
    return tokens;
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case '*': addToken(TokenType::STAR); break;
        case '/': addToken(TokenType::SLASH); break;
        case '[': addToken(TokenType::LEFT_BRACKET); break;
        case ']': addToken(TokenType::RIGHT_BRACKET); break;
        case '!': 
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); 
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '#':
            while (peek() != '\n' && !isAtEnd()) advance();
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            addToken(TokenType::NEWLINE);
            line++;
            break;
        case '"': scanString(); break;
        default:
            if (isDigit(c)) {
                scanNumber();
            } else if (isAlpha(c)) {
                scanIdentifier();
            } else {
                HPI::error(line, format("Unexpected character {}.", c));
            }
            break;
    }
}

void Scanner::scanIdentifier() {
    while (isAlphaNumeric(peek())) advance();

    string text = src.substr(start, current - start);
    const bool isReservedKeyword =
        keywords.find(text) != keywords.end();
    
    TokenType type = isReservedKeyword ? keywords[text] : TokenType::IDENTIFIER;
    addToken(type);
}

void Scanner::scanNumber() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        while (isDigit(peek())) advance();
    }

    addToken(TokenType::NUMBER, stod(src.substr(start, current - start)));
}

void Scanner::scanString() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
        HPI::error(line, "Unterminated string.");
        return;
    }

    advance();

    string value = src.substr(start + 1, current - 2 - start);
    addToken(TokenType::STRING, value);
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (src[current] != expected) return false;

    current++;
    return true;
}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return src[current];
}

char Scanner::peekNext() {
    if (current + 1 >= src.length()) return '\0';
    return src[current + 1];
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z') || 
            c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::isAtEnd() {
    return current >= src.length();
}

char Scanner::advance() {
    return src[current++];
}

void Scanner::addToken(TokenType type) {
    addToken(type, "");
}

void Scanner::addToken(TokenType type, any literal) {
    string text = src.substr(start, current - start);
    tokens.push_back(Token(type, text, literal, line));
}
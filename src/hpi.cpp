#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <format>

#include "hpi.hpp"
#include "scanner/scanner.hpp"
#include "parser/parser.hpp"
#include "interpreter/interpreter.hpp"

// Global variables
bool hadError = false;
bool hadRuntimeError = false;
Interpreter interpreter;

// Namespace definitions
void HPI::runFile(string path) {
    ifstream file(path);
    ostringstream byte_stream;
    byte_stream << file.rdbuf();
    run(byte_stream.str());

    if (hadError) exit(65);
    if (hadRuntimeError) exit(70);
}

void HPI::runPrompt() {
    string line;
    
    while (true) {
        cout << ".>> ";
        getline(cin, line);
        if (line.empty()) break;

        Scanner scanner(line);
        vector<Token> tokens = scanner.scanTokens();

        Parser parser(tokens);
        any syntax = parser.parseRepl();

        if (hadError) continue;

        if (syntax.type() == typeid(vector<Stmt*>)) {
            interpreter.interpret(any_cast<vector<Stmt*>>(syntax));
        } else if (syntax.type() == typeid(Expr*)) {
            string result = interpreter.interpret(any_cast<Expr*>(syntax));
            if (result != "expr null") {
                cout << ":= " << result << endl;
            }
        }

        hadError = false;
    }
}

void HPI::run(string src) {
    Scanner scanner(src);
    vector<Token> tokens = scanner.scanTokens();

    Parser parser(tokens);
    vector<Stmt*> statements = parser.parse();

    if (hadError) return;

    interpreter.interpret(statements);
}

void HPI::error(int line, string msg) {
    report(line, "", msg);
}

void HPI::error(Token token, string msg) {
    if (token.type == TokenType::E_O_F) {
        report(token.line, " at end", msg);
    } else if (token.type == TokenType::NEWLINE) {
        report(token.line, " at newline", msg);
    } else {
        report(token.line, format(" at '{}'", token.lexeme), msg);
    }
}

void HPI::runtimeError(RuntimeError e) {
    cerr << format("[Line {}] Error: {}", e.token.line, e.what()) << endl;
    hadRuntimeError = true;
}

void HPI::report(int line, string where, string msg) {
    cerr << format("[Line {}] Error {}: {}", line, where, msg) << endl;
    hadError = true;
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        cerr << "Usage: hpi [file]" << endl;
        exit(64);
    } else if (argc == 2) {
        HPI::runFile(argv[1]);
    } else {
        HPI::runPrompt();
    }
    return 0;
}
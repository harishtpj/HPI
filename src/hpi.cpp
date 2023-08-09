#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <format>

#include "hpi.hpp"
#include "scanner/scanner.hpp"
#include "parser/parser.hpp"
#include "tools/astPrint.hpp"

// Global variables
bool hadError = false;

// Namespace definitions
void HPI::runFile(string path) {
    ifstream file(path);
    ostringstream byte_stream;
    byte_stream << file.rdbuf();
    run(byte_stream.str());

    if (hadError) exit(65);
}

void HPI::runPrompt() {
    string line;
    
    while (true) {
        cout << ".>> ";
        getline(cin, line);
        if (line.empty()) break;
        run(line);
        hadError = false;
    }
}

void HPI::run(string src) {
    Scanner scanner(src);
    vector<Token> tokens = scanner.scanTokens();

    Parser parser(tokens);
    Expr* expression = parser.parse();

    if (hadError) return;

    ASTPrinter printer;
    printer.print(expression);
    cout << endl;
}

void HPI::error(int line, string msg) {
    report(line, "", msg);
}

void HPI::error(Token token, string msg) {
    if (token.type == TokenType::E_O_F) {
        report(token.line, " at end", msg);
    } else {
        report(token.line, format(" at '{}'", token.lexeme), msg);
    }
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
        HPI::runFile(argv[0]);
    } else {
        HPI::runPrompt();
    }
    return 0;
}
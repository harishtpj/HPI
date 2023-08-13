#pragma once

#include <string>

#include "scanner/token.hpp"
#include "interpreter/runtime_err.hpp"

using namespace std;

namespace HPI {
    void runFile(string path);
    void runPrompt();
    void run(string src);
    void error(int line, string msg);
    void error(Token token, string msg);
    void runtimeError(RuntimeError e);
    void report(int line, string where, string msg);
}
#pragma once

#include "interpreter.hpp"

class HPIFunction: public HPICallable {
    public:
        HPIFunction(FunctionStmt* declaration, Environment* closure);
        int arity();
        any call(Interpreter& interpreter, vector<any> arguments);
        string toString();

    private:
        FunctionStmt* declaration;
        Environment* closure;
};
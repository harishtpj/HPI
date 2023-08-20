#include "HPIFunction.hpp"
#include <iostream>

HPIFunction::HPIFunction(FunctionStmt* declaration, Environment* closure): 
    declaration(declaration),
    closure(closure) {}

int HPIFunction::arity() {
    return declaration->params.size();
}

any HPIFunction::call(Interpreter& interpreter, vector<any> arguments) {
    Environment* environment = new Environment(closure);
            
    for(int i = 0; i < declaration->params.size(); i++) {
        environment->define(declaration->params[i].lexeme, arguments[i]);
    }

    try {
        interpreter.executeBlock(declaration->body, environment);
    } catch (const ReturnException& e) {
        return e.value;
    }
    return nullptr;
}

string HPIFunction::toString() {
    return format("<UserDef Fn {}>", declaration->name.lexeme);
}
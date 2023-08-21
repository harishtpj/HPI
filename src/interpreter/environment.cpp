#include "environment.hpp"
#include "runtime_err.hpp"

Environment::Environment(): enclosing(nullptr) {}
Environment::Environment(Environment* enclosing): enclosing(enclosing) {}

void Environment::define(string name, any value) {
    values[name] = value;
}

Environment* Environment::ancestor(int distance) {
    Environment* environment = this;
    for (int i = 0; i < distance; i++) {
        environment = environment->enclosing;
    }
    return environment;
}

any Environment::get(Token name) {
    if (values.find(name.lexeme) != values.end()) {
        return values[name.lexeme];
    }

    if (enclosing != nullptr) return enclosing->get(name);

    throw RuntimeError(name, "Undefined identifier '" + name.lexeme + "'.");
}

any Environment::getAt(int distance, string name) {
    return ancestor(distance)->values[name];
}

void Environment::assign(Token name, any value) {
    if (values.find(name.lexeme) != values.end()) {
        values[name.lexeme] = value;
        return;
    }

    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::assignAt(int distance, Token name, any value) {
    ancestor(distance)->values[name.lexeme] = value;
}
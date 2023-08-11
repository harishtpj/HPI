#pragma once

#include "../scanner/token.hpp"

class RuntimeError : public runtime_error {
    public:
        Token token;
        RuntimeError(Token token, string msg): runtime_error(msg), token(token) {}
};
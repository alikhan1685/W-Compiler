#pragma once
#include "Token.h"
#include <string>
#include <unordered_map>
#include <stdexcept>
//  Data  —  symbol table (mirrors Python's data class)
class Data {
public:
    // read: returns the Token stored under 'id'.
    // Throws if the variable has not been declared.
    Token read(const std::string& id) const {
        auto it = variables_.find(id);
        if (it == variables_.end())
            throw std::runtime_error("Undefined variable: " + id);
        return it->second;
    }
    // read_all: returns a const reference to the whole symbol table.
    const std::unordered_map<std::string, Token>& read_all() const {
        return variables_;
    }
    // write: stores 'expression' under variable.value as the key.
    void write(const Token& variable, const Token& expression) {
        variables_[variable.value] = expression;
    }
private:
    std::unordered_map<std::string, Token> variables_;
};
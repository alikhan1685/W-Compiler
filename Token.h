#pragma once
#include <string>
#include <ostream>
#include <unordered_map>

struct Token {
    std::string type;
    std::string value;
    int line;
    int column;

    Token() : type("EOF"), value(""), line(0), column(0) {}
    Token(std::string t, std::string v, int l = 0, int c = 0)
        : type(std::move(t)), value(std::move(v)), line(l), column(c) {}

    bool isEOF() const { return type == "EOF"; }

    friend std::ostream& operator<<(std::ostream& os, const Token& t) {
        return os << t.value;
    }
};

// KEYWORDS: W++ language ke reserved keywords ka map
const std::unordered_map<std::string, std::string> KEYWORDS = {
    // Variable declaration and assignment
    {"let", "DECLARATION"},
    {"set", "ASSIGN"},
    
    // Conditional statements
    {"check", "IF"},
    {"otherwise check", "ELIF"},
    {"otherwise if", "ELSEIF"},
    {"otherwise", "ELSE"},
    
    // Loops and blocks
    {"do", "DO"},
    {"repeat till", "WHILE"},
    
    // Logical operators
    {"and", "AND"},
    {"or", "OR"},
    {"not", "NOT"},
    
    // Input/Output keywords
    {"i/p", "INPUT"},
    {"scan", "INPUT"},
    {"o/p", "OUTPUT"},
    {"print", "OUTPUT"},
    
    // Program control
    {"end", "END"},
    {"return", "RETURN"},
    {"main", "MAIN"},
    
    // ===== MATH LIBRARY KEYWORDS =====
    {"use", "USE"},
    {"math", "MATH"},
    {"sqrt", "SQRT"},
    {"pow", "POW"},
    {"ceil", "CEIL"},
    {"floor", "FLOOR"},
    {"fabs", "FABS"},
    {"factorial", "FACTORIAL"}
};
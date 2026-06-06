#pragma once
#include <string>
#include <ostream>
#include <unordered_map>
// Token struct: Lexer ka output, source code ka ek unit
// Har token mein type (INT, VARIABLE, OPERATOR, etc.), value, aur position (line, column) hoti hai
struct Token {
    std::string type;    // Token ki type: "INT", "VARIABLE", "OPERATOR", "IF", etc.
    std::string value;   // Token ki actual value: "123", "x", "+", "if", etc.
    int line;            // Source code mein line number (error reporting ke liye)
    int column;          // Source code mein column number (error reporting ke liye)
    // Default constructor: EOF token banata hai
    Token() : type("EOF"), value(""), line(0), column(0) {}  
    // Parameterized constructor: Token banata hai with type, value, aur position
    Token(std::string t, std::string v, int l = 0, int c = 0)
        : type(std::move(t)), value(std::move(v)), line(l), column(c) {}
    // isEOF(): Check karta hai ke yeh End-Of-File token hai ya nahi
    bool isEOF() const { return type == "EOF"; }
    // Operator<<: Token ko easily print karne ke liye (cout << token)
    friend std::ostream& operator<<(std::ostream& os, const Token& t) {
        return os << t.value;
    }
};
// KEYWORDS: W++ language ke reserved keywords ka map
const std::unordered_map<std::string, std::string> KEYWORDS = {
    {"notekar", "DECLARATION"},
    {"dekhbhai", "IF"},
    {"elif", "ELIF"},
    {"warnaboss", "ELSEIF"},
    {"achaphir", "ELSE"},
    {"kar", "DO"},
    {"lagayraho", "WHILE"},
    {"sath", "AND"},
    {"yaye", "OR"},
    {"nahi", "NOT"}
};
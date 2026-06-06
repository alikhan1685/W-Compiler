#pragma once
// Memory management aur string handling ke liye libraries include ki gayi hain
#include <memory>
#include <string>
// Token aur AST ki definitions wali header files include ki gayi hain
#include "Token.h"
#include "AST.h"
// Integer token banane ke liye helper function
inline Token makeInt(const std::string& v) {
    return Token{"Int", v};
}
// Float token banane ke liye helper function
inline Token makeFloat(const std::string& v) {
    return Token{"Float", v};
}
// Variable token banane ke liye helper function
inline Token makeVar(const std::string& v) {
    return Token{"Variable", v};
}
// AST ka leaf node create karne ke liye function
// Token ko ASTNode mein convert karke shared pointer return karta hai
inline ASTNodePtr leaf(Token t) {
    return std::make_shared<ASTNode>(t);
}
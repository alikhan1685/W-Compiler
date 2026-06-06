#pragma once
#include <memory>
#include <vector>
#include "Token.h"
// ASTNode: Abstract Syntax Tree ka node
// Ye do qism ka ho sakta hai:
// 1) Leaf node (isList = false): ek single token rakhta hai, jaise number, variable, ya operator
// 2) List node (isList = true): children mein doosray AST nodes ka collection rakhta hai, jaise function call ya block
struct ASTNode {
    bool isList;                         // kya ye node ek list hai? (true = list, false = token/leaf)
    Token token;                         // agar leaf node hai to ye token store karega
    std::vector<std::shared_ptr<ASTNode>> children;  // agar list node hai to is mein children honge
    int line;                            // source code mein line number (position tracking ke liye)
    int column;                          // source code mein column numbe
    // Constructor: Token-based node (leaf node)
    ASTNode(Token t) : isList(false), token(t), line(t.line), column(t.column) {}
    // Constructor: List-based node (internal node)
    ASTNode(std::vector<std::shared_ptr<ASTNode>> c, int l = 0, int col = 0)
        : isList(true), children(std::move(c)), line(l), column(col) {}
};
// ASTNodePtr: ASTNode ka shared pointer, memory management aur sharing ke liye aasan
using ASTNodePtr = std::shared_ptr<ASTNode>;
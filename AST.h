#pragma once
#include <memory>
#include <vector>
#include "Token.h"

struct ASTNode {
    bool isList;
    Token token;
    std::vector<std::shared_ptr<ASTNode>> children;
    int line;
    int column;

    ASTNode(Token t) : isList(false), token(t), line(t.line), column(t.column) {}
    ASTNode(std::vector<std::shared_ptr<ASTNode>> c, int l = 0, int col = 0)
        : isList(true), children(std::move(c)), line(l), column(col) {}
};

using ASTNodePtr = std::shared_ptr<ASTNode>;
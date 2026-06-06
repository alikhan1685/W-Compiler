#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cmath>
#include "Token.h"
#include "AST.h"

using namespace std;

inline ASTNodePtr leafNode(Token t) {
    return make_shared<ASTNode>(t);
}

inline ASTNodePtr listNode(vector<ASTNodePtr> kids) {
    return make_shared<ASTNode>(std::move(kids));
}

class Parser {
private:
    vector<Token> tokens_;
    size_t idx_;
    Token token_;
    bool useMathLibrary = false;

public:
    explicit Parser(vector<Token> tokens) : tokens_(std::move(tokens)), idx_(0) {
        token_ = tokens_.empty() ? Token("EOF", "") : tokens_[0];
    }

    ASTNodePtr parse() {
        cout << string(60, '=') << endl;
        cout << "PARSING PHASE" << endl;
        cout << string(60, '=') << endl;
        
        // Check for #use math directive
        if (token_.type == "USE") {
            move(); // skip '#use'
            if (token_.value == "math") {
                useMathLibrary = true;
                cout << "  -> Math library loaded!" << endl;
                move(); // skip 'math'
            }
        }
        
        // Check for main function
        if (token_.type == "MAIN") {
            cout << "  -> Found main() function" << endl;
            move(); // skip 'main'
            
            if (token_.value != "(") {
                throw runtime_error("Expected '(' after main");
            }
            move(); // skip '('
            
            if (token_.value != ")") {
                throw runtime_error("Expected ')' after main(");
            }
            move(); // skip ')'
            
            if (token_.value != "{") {
                throw runtime_error("Expected '{' after main()");
            }
            
            // Parse main block
            ASTNodePtr mainBlock = block();
            cout << "Statements parsed: " << mainBlock->children.size() << endl;
            cout << string(60, '=') << "\n" << endl;
            return mainBlock;
        }
        
        // No main function - parse all statements as program
        vector<ASTNodePtr> program;
        while (token_.type != "EOF" && token_.type != "END") {
            if (idx_ >= tokens_.size()) break;
            program.push_back(statement());
            if (token_.type == "END") {
                move();
                break;
            }
        }
        
        cout << "Statements parsed: " << program.size() << endl;
        cout << string(60, '=') << "\n" << endl;
        return listNode(program);
    }

private:
    void move() {
        idx_++;
        if (idx_ < tokens_.size()) token_ = tokens_[idx_];
    }

    ASTNodePtr parseMathFunction() {
        if (token_.type == "MATH") {
            Token mathToken = token_;
            move(); // skip 'math'
            
            if (token_.value != ".") {
                throw runtime_error("Expected '.' after math");
            }
            move(); // skip '.'
            
            if (token_.type == "SQRT") {
                Token func = token_;
                move();
                if (token_.value != "(") throw runtime_error("Expected '(' after sqrt");
                move();
                ASTNodePtr arg = expression();
                if (token_.value != ")") throw runtime_error("Expected ')'");
                move();
                return listNode({leafNode(mathToken), leafNode(func), arg});
            }
            else if (token_.type == "POW") {
                Token func = token_;
                move();
                if (token_.value != "(") throw runtime_error("Expected '(' after pow");
                move();
                ASTNodePtr base = expression();
                if (token_.value != ",") throw runtime_error("Expected ','");
                move();
                ASTNodePtr exponent = expression();
                if (token_.value != ")") throw runtime_error("Expected ')'");
                move();
                return listNode({leafNode(mathToken), leafNode(func), base, exponent});
            }
            else if (token_.type == "CEIL") {
                Token func = token_;
                move();
                if (token_.value != "(") throw runtime_error("Expected '(' after ceil");
                move();
                ASTNodePtr arg = expression();
                if (token_.value != ")") throw runtime_error("Expected ')'");
                move();
                return listNode({leafNode(mathToken), leafNode(func), arg});
            }
            else if (token_.type == "FLOOR") {
                Token func = token_;
                move();
                if (token_.value != "(") throw runtime_error("Expected '(' after floor");
                move();
                ASTNodePtr arg = expression();
                if (token_.value != ")") throw runtime_error("Expected ')'");
                move();
                return listNode({leafNode(mathToken), leafNode(func), arg});
            }
            else if (token_.type == "FABS") {
                Token func = token_;
                move();
                if (token_.value != "(") throw runtime_error("Expected '(' after fabs");
                move();
                ASTNodePtr arg = expression();
                if (token_.value != ")") throw runtime_error("Expected ')'");
                move();
                return listNode({leafNode(mathToken), leafNode(func), arg});
            }
            else if (token_.type == "FACTORIAL") {
                Token func = token_;
                move();
                if (token_.value != "(") throw runtime_error("Expected '(' after factorial");
                move();
                ASTNodePtr arg = expression();
                if (token_.value != ")") throw runtime_error("Expected ')'");
                move();
                return listNode({leafNode(mathToken), leafNode(func), arg});
            }
        }
        return nullptr;
    }

    ASTNodePtr factor() {
        // Check for math function calls
        if (useMathLibrary && token_.type == "MATH") {
            ASTNodePtr mathNode = parseMathFunction();
            if (mathNode) return mathNode;
        }
        
        if (token_.type == "INT" || token_.type == "FLOAT" || token_.type == "STRING") {
            auto node = leafNode(token_);
            move();
            return node;
        }
        
        if (token_.type == "VARIABLE") {
            auto node = leafNode(token_);
            move();
            return node;
        }
        
        if (token_.value == "(") {
            move();
            auto node = expression();
            if (token_.value == ")") move();
            return node;
        }
        
        throw runtime_error("Invalid factor: " + token_.value);
    }

    ASTNodePtr term() {
        auto left = factor();
        while (token_.value == "*" || token_.value == "/") {
            Token op = token_;
            move();
            auto right = factor();
            left = listNode({leafNode(op), left, right});
        }
        return left;
    }

    ASTNodePtr expression() {
        auto left = term();
        while (token_.value == "+" || token_.value == "-") {
            Token op = token_;
            move();
            auto right = term();
            left = listNode({leafNode(op), left, right});
        }
        return left;
    }

    ASTNodePtr comparison() {
        auto left = expression();
        while (token_.type == "COMPARISON") {
            Token op = token_;
            move();
            auto right = expression();
            left = listNode({leafNode(op), left, right});
        }
        return left;
    }

    ASTNodePtr booleanExpr() {
        auto left = comparison();
        while (token_.type == "AND" || token_.type == "OR") {
            Token op = token_;
            move();
            auto right = comparison();
            left = listNode({leafNode(op), left, right});
        }
        if (token_.type == "NOT") {
            Token op = token_;
            move();
            auto expr = booleanExpr();
            left = listNode({leafNode(op), expr});
        }
        return left;
    }

    ASTNodePtr block() {
        vector<ASTNodePtr> statements;
        if (token_.value == "{") move();
        
        while (token_.value != "}" && token_.type != "EOF" && token_.type != "END") {
            statements.push_back(statement());
        }
        if (token_.value == "}") move();
        
        return listNode(statements);
    }

    ASTNodePtr statement() {
        // DECLARATION
        if (token_.type == "DECLARATION") {
            Token decl = token_;
            move();
            if (token_.type != "VARIABLE") throw runtime_error("Expected variable name");
            Token var = token_;
            move();
            if (token_.value != "=") throw runtime_error("Expected '='");
            move();
            ASTNodePtr right = expression();
            return listNode({leafNode(decl), leafNode(var), right});
        }
        
        // ASSIGNMENT
        if (token_.type == "ASSIGN") {
            move();
            if (token_.type != "VARIABLE") throw runtime_error("Expected variable name");
            Token var = token_;
            move();
            if (token_.value != "=") throw runtime_error("Expected '='");
            move();
            ASTNodePtr right = expression();
            return listNode({leafNode(var), leafNode(Token("OPERATOR", "=")), right});
        }
        
        // INPUT
        if (token_.type == "INPUT") {
            Token inputKw = token_;
            move();
            if (token_.type != "VARIABLE") throw runtime_error("Expected variable name");
            Token var = token_;
            move();
            return listNode({leafNode(inputKw), leafNode(var)});
        }
        
        // OUTPUT
        if (token_.type == "OUTPUT") {
            Token outputKw = token_;
            move();
            ASTNodePtr expr = expression();
            return listNode({leafNode(outputKw), expr});
        }
        
        // IF
        if (token_.type == "IF") {
            Token kw = token_;
            move();
            ASTNodePtr condition = booleanExpr();
            ASTNodePtr body = block();
            vector<ASTNodePtr> ifNode = {leafNode(kw), condition, body};
            while (token_.type == "ELIF" || token_.type == "ELSEIF") {
                Token elifKw = token_;
                move();
                ASTNodePtr elifCond = booleanExpr();
                ASTNodePtr elifBody = block();
                ifNode.push_back(listNode({leafNode(elifKw), elifCond, elifBody}));
            }
            if (token_.type == "ELSE") {
                move();
                ASTNodePtr elseBody = block();
                ifNode.push_back(listNode({leafNode(Token("ELSE", "otherwise")), elseBody}));
            }
            return listNode(ifNode);
        }

        // WHILE
        if (token_.type == "WHILE") {
            Token kw = token_;
            move();
            ASTNodePtr condition = booleanExpr();
            ASTNodePtr body = block();
            return listNode({leafNode(kw), condition, body});
        }

        // END
        if (token_.type == "END") {
            Token endKw = token_;
            move();
            return leafNode(endKw);
        }

        // Expression
        ASTNodePtr expr = expression();
        return expr;
    }
};
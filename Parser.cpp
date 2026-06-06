#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "Token.h"
#include "AST.h"
using namespace std;
// leafNode: Token se ek leaf (terminal) AST node banata hai
// Ye numbers, variables, strings jaise terminal values ke liye use hota hai
inline ASTNodePtr leafNode(Token t) {
    return make_shared<ASTNode>(t);
}
// listNode: Children ki list se ek internal AST node banata hai
// Ye operators, blocks, if-statements jaise non-terminal nodes ke liye use hota hai
inline ASTNodePtr listNode(vector<ASTNodePtr> kids) {
    return make_shared<ASTNode>(std::move(kids));
}
// Parser class: Tokens ko Abstract Syntax Tree (AST) mein convert karta hai
// Ye recursive descent parser hai jo grammar rules follow karta hai
class Parser {
public:
    // Constructor: Tokens ki list leta hai aur parser ko initialize karta hai
    explicit Parser(vector<Token> tokens) : tokens_(std::move(tokens)), idx_(0) {
        token_ = tokens_.empty() ? Token("EOF", "") : tokens_[0];
    }
    // parse(): Poori program ka AST banata hai
    // Program mein statements ki list hoti hai
    ASTNodePtr parse() {
        cout << string(60, '=') << endl;
        cout << "PARSING PHASE" << endl;
        cout << string(60, '=') << endl;
        
        vector<ASTNodePtr> program;  // Program ke saare statements store honge
        int stmtCount = 0;
        
        // Jab tak end of file nahi aa jata, statements parse karte raho
        while (token_.type != "EOF") {
            if (idx_ >= tokens_.size()) break;
            cout << "Parsing statement " << (stmtCount + 1) << endl;
            program.push_back(statement());  // Har statement ko parse karo
            stmtCount++;
        }
        
        cout << "Statements parsed: " << program.size() << endl;
        cout << string(60, '=') << "\n" << endl;
        
        ASTNodePtr root = listNode(program);  // Root node banao
        cout << "DEBUG: Root node has " << root->children.size() << " children" << endl;
        
        return root;
    }

private:
    vector<Token> tokens_;  // Tokens ki list (input)
    size_t idx_;            // Current position in tokens
    Token token_;           // Current token

    // move(): Next token par move karta hai
    void move() {
        idx_++;
        if (idx_ < tokens_.size()) token_ = tokens_[idx_];
    }

    // factor(): Lowest level expressions parse karta hai
    // Numbers, variables, strings, ya parentheses mein expressions handle karta hai
    ASTNodePtr factor() {
        // Agar INT, FLOAT, ya STRING hai to leaf node banao
        if (token_.type == "INT" || token_.type == "FLOAT" || token_.type == "STRING") {
            auto node = leafNode(token_);
            move();
            return node;
        }
        // Agar VARIABLE hai to leaf node banao
        if (token_.type == "VARIABLE") {
            auto node = leafNode(token_);
            move();
            return node;
        }
        // Agar '(' hai to parentheses mein expression handle karo
        if (token_.value == "(") {
            move();
            auto node = expression();
            if (token_.value == ")") move();
            return node;
        }
        throw runtime_error("Invalid factor: " + token_.value);
    }

    // term(): Multiplication aur division handle karta hai
    // Left-associative: a * b / c → ((a * b) / c)
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

    // expression(): Addition aur subtraction handle karta hai
    // Term se banta hai, operator precedence handle karta hai (*/ > +-)
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

    // comparison(): Comparison operators handle karta hai (>, <, ==, etc.)
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

    // booleanExpr(): Logical operators handle karta hai (AND, OR)
    // Future expansion ke liye ready
    ASTNodePtr booleanExpr() {
        auto left = comparison();
        while (token_.type == "AND" || token_.type == "OR") {
            Token op = token_;
            move();
            auto right = comparison();
            left = listNode({leafNode(op), left, right});
        }
        return left;
    }

    // block(): Curly braces { ... } ke andar statements ka group parse karta hai
    ASTNodePtr block() {
        vector<ASTNodePtr> statements;
        if (token_.value == "{") move();  // Opening brace skip karo
        
        // Jab tak closing brace nahi aa jata, statements parse karte raho
        while (token_.value != "}" && token_.type != "EOF") {
            statements.push_back(statement());
        }
        if (token_.value == "}") move();  // Closing brace skip karo
        
        return listNode(statements);
    }

    // statement(): Har type ka statement parse karta hai
    // Declaration, assignment, if, while, ya simple expression
    ASTNodePtr statement() {
        // ===== DECLARATION: notekar var = value =====
        if (token_.type == "DECLARATION") {
            Token decl = token_;
            move();
            
            if (token_.type != "VARIABLE") {
                throw runtime_error("Expected variable name");
            }
            Token var = token_;
            move();
            
            if (token_.value != "=") {
                throw runtime_error("Expected '='");
            }
            move();
            
            ASTNodePtr right = expression();
            cout << "  -> Parsed DECLARATION: " << var.value << endl;
            return listNode({leafNode(decl), leafNode(var), right});
        }
        
        // ===== ASSIGNMENT: var = value (bina notekar ke) =====
        if (token_.type == "VARIABLE") {
            size_t nextIdx = idx_ + 1;
            if (nextIdx < tokens_.size() && tokens_[nextIdx].value == "=") {
                Token var = token_;
                move(); // move past variable
                move(); // move past '='
                ASTNodePtr right = expression();
                cout << "  -> Parsed ASSIGNMENT: " << var.value << endl;
                return listNode({leafNode(var), leafNode(Token("OPERATOR", "=")), right});
            }
        }
        
        // ===== IF STATEMENT =====
        if (token_.type == "IF") {
            Token kw = token_;
            move();
            ASTNodePtr condition = booleanExpr();  // Condition parse karo
            ASTNodePtr body = block();             // Body parse karo
            
            vector<ASTNodePtr> ifNode = {leafNode(kw), condition, body};
            
            // Optional ELIF / ELSEIF blocks parse karo
            while (token_.type == "ELIF" || token_.type == "ELSEIF") {
                Token elifKw = token_;
                move();
                ASTNodePtr elifCond = booleanExpr();
                ASTNodePtr elifBody = block();
                ifNode.push_back(listNode({leafNode(elifKw), elifCond, elifBody}));
            }
            
            // Optional ELSE block parse karo
            if (token_.type == "ELSE") {
                Token elseKw = token_;
                move();
                ASTNodePtr elseBody = block();
                ifNode.push_back(listNode({leafNode(elseKw), elseBody}));
            }
            
            cout << "  -> Parsed IF statement" << endl;
            return listNode(ifNode);
        }

        // ===== WHILE STATEMENT =====
        if (token_.type == "WHILE") {
            Token kw = token_;
            move();
            ASTNodePtr condition = booleanExpr();  // Condition parse karo
            ASTNodePtr body = block();             // Body parse karo
            cout << "  -> Parsed WHILE loop" << endl;
            return listNode({leafNode(kw), condition, body});
        }
        // ===== EXPRESSION (variable reference ya calculation) =====
        ASTNodePtr expr = expression();
        if (expr->token.type == "VARIABLE") {
            cout << "  -> Parsed VARIABLE reference: " << expr->token.value << endl;
        } else {
            cout << "  -> Parsed EXPRESSION" << endl;
        }
        return expr;
    }
};
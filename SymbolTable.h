#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Token.h"

class SymbolTable {
private:
    struct Symbol {
        Token value;
        int declaredAt;
        bool initialized;
        std::string type;
    };
    
    std::unordered_map<std::string, Symbol> table;
    std::vector<std::string> declarationOrder;
    
public:
    void declare(const std::string& name, const Token& value, int line) {
        if (table.find(name) != table.end()) {
            table[name].value = value;
            table[name].initialized = true;
            table[name].type = value.type;
            std::cout << "  [SYMBOL] Updated " << name << " = " << value.value << std::endl;
            return;
        }
        
        table[name] = {value, line, true, value.type};
        declarationOrder.push_back(name);
        std::cout << "  [SYMBOL] Declared " << name << " = " << value.value << std::endl;
    }
    
    void write(const std::string& name, const Token& value) {
        if (table.find(name) == table.end()) {
            throw std::runtime_error("Undeclared variable: " + name);
        }
        table[name].value = value;
        table[name].initialized = true;
        table[name].type = value.type;
        std::cout << "  [SYMBOL] Wrote " << name << " = " << value.value << std::endl;
    }
    
    Token read(const std::string& name) const {
        auto it = table.find(name);
        if (it == table.end()) {
            throw std::runtime_error("Undeclared variable: " + name);
        }
        return it->second.value;
    }
    
    bool exists(const std::string& name) const {
        return table.find(name) != table.end();
    }
    
    void printSymbolTable() const {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "SYMBOL TABLE" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << std::left << std::setw(15) << "Name" 
                  << std::setw(10) << "Type" 
                  << std::setw(15) << "Value" 
                  << "Line" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (const auto& name : declarationOrder) {
            auto it = table.find(name);
            if (it != table.end()) {
                std::cout << std::left << std::setw(15) << name
                          << std::setw(10) << it->second.type
                          << std::setw(15) << it->second.value.value
                          << it->second.declaredAt << std::endl;
            }
        }
        std::cout << std::string(60, '=') << "\n" << std::endl;
    }
    
    void printAllValues() const {
        std::cout << "\nFINAL VARIABLE VALUES:" << std::endl;
        std::cout << std::string(40, '-') << std::endl;
        for (const auto& name : declarationOrder) {
            auto it = table.find(name);
            if (it != table.end()) {
                std::cout << name << " = " << it->second.value.value << std::endl;
            }
        }
        std::cout << std::string(40, '-') << std::endl;
    }
};
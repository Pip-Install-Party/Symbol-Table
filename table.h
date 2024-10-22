#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include "token.h"
#include "entry.h"

const std::vector<std::string> reserved = {"printf", "int", "void", "char", "bool", "string", "procedure", "function"};

class Table {
    private:
    Entry* head;
    bool pause = false;
    short scope = 0;
    bool contains(std::string);
    void build(Token*, Token*, Entry*);
    void setArray(Token*, Entry*);
    void handleInitList(std::string, Token*, Entry*);
    public:
    Table(){};
    void begin(Token*);
    void printTable();
    ~Table(){};
};

#endif // TABLE_H
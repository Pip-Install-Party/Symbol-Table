#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include "token.h"
#include "entry.h"
#include <vector>

const std::vector<std::string> reserved = {"printf", "int", "void", "char", "bool", "string", "procedure", "function"};

class Table {
    private:
    Entry* head = nullptr;
    bool pause = false;
    short scope = 0;
    bool inProcOrFuncScope = false;
    bool contains(std::string);
    void exists(Token*, int);
    void build(Token*, Token*, Entry*);
    void setArray(Token*, Entry*);
    void handleInitList(std::string, Token*, Entry*);
    // Token* handleParameterList(Entry*, Token*);
    public:
    Table(){};
    void begin(Token*);
    void printTable();
    void printParameters();
    ~Table(){};
};

#endif // TABLE_H
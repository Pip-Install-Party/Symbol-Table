#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include "token.h"
#include "entry.h"

class Table {
    private:
    Entry* head;
    void build(Token*, Entry*);
    public:
    Table(){};
    void begin(Token*);
    void printTable();
    ~Table(){};
};

#endif // TABLE_H
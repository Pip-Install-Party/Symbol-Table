#include <iostream>
#include "table.h"

void Table::begin(Token* token){
    Entry* entry = new Entry(token->getValue(), "Not Implemented", token->getType(), false, 
            0, 0); 
    head = entry;
    if (token->getSibling() != nullptr) {
            token = token->getSibling();
        } else if (token->getChild() != nullptr) {
            token = token->getChild();
        } else {
            return; 
    }
    return build(token, entry);
}

void Table::build(Token* token, Entry* prevEntry){
    
    Entry* entry = new Entry(token->getValue(), "Not Implemented", token->getType(), false, 
    0, 0); 
    prevEntry->setNext(entry);
    if (token->getSibling() != nullptr) {
        token = token->getSibling();
    } else if (token->getChild() != nullptr) {
        token = token->getChild();
    } else {
        return; 
    }
    return build(token, entry);
 }



void Table::printTable(){
    while(head != nullptr) {
        std::cout << "IDENTIFIER_NAME: " << head->getIDName() << std::endl;
        std::cout << "IDENTIFIER_TYPE: " << head->getDType() << std::endl;
        std::cout << "DATATYPE: " << head->getDType() << std::endl;
        std::cout << "DATATYPE_IS_ARRAY: ";
        head->getIsArray() ?  std::cout << "yes" :  std::cout << "no";
        std::cout << "\nDATATYPE_ARRAY_SIZE: " << head->getArraySize() << std::endl;
        std::cout << "SCOPE: " << head->getScope() << std::endl << std::endl;
        head = head->getNext();
    }
}
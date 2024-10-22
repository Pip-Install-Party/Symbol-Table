#include <iostream>
#include "table.h"

void Table::begin(Token* token){
    if (token->getType() == "IDENTIFIER") {
        Entry* tempEntry = nullptr;
        if (token->getSibling() != nullptr) {
            return build(token->getSibling(), token, tempEntry);
        } else if (token->getChild() != nullptr) {
            token = token->getChild();
            return build(token->getChild(), token, tempEntry);
        } else {
                return; 
        }
    } else {
         if (token->getSibling() != nullptr) {
                begin(token->getSibling());
            } else if (token->getChild() != nullptr) {
                begin(token->getChild());
            } else {
                return; 
        }
    }
}

void Table::build(Token* token, Token* prevToken, Entry* prevEntry) {
    if (prevToken == nullptr || token == nullptr) {
        return; // Ensure the tokens are valid before proceeding
    }
    Entry* entry = prevEntry;  // Start with the previous entry

    if (!pause && contains(prevToken->getValue()) && token->getValue() != "{" && token->getValue() != "}" && token->getValue() != "(" && token->getValue() != ")" && token->getValue() != "[" && token->getValue() != "]") {
        // Create a new entry based on the token values
        pause = true;
        if (prevToken->getValue() == "procedure" || prevToken->getValue() == "function") {
            scope++;
            if (prevToken->getValue() == "procedure") {
                entry = new Entry(token->getValue(), prevToken->getValue(), "NOT APPLICABLE", false, 0, scope);
                if (prevEntry == nullptr ){
                    head = entry;
                } 
            } else {
                entry = new Entry(token->getSibling()->getValue(), prevToken->getValue(), token->getValue(), false, 0, scope);
                if (prevEntry == nullptr ){
                    head = entry;
                } else {
                    prevEntry->setNext(entry);  // Update the next pointer of prevEntry
                }
                return build(token->getSibling()->getSibling(), token->getSibling(), entry );
            }
        } else {
            entry = new Entry(token->getValue(), "datatype", prevToken->getValue(), false, 0, scope);
             if (prevEntry == nullptr ){
                head = entry;
            } 
            if (token->getSibling() != nullptr) {

                if (token->getSibling()->getValue() == "[") { 
                    if ( prevEntry != nullptr ) {
                        prevEntry->setNext(entry);  // Update the next pointer of prevEntry
                    }
                   return setArray(token->getSibling(), entry);
                } else if (token->getSibling()->getValue() == ",") { 
                    if ( prevEntry != nullptr ) {
                        prevEntry->setNext(entry);  // Update the next pointer of prevEntry
                    }
                    return handleInitList(prevToken->getValue(), token->getSibling()->getSibling(), entry);
                }
            }
        }
        if ( prevEntry != nullptr ) {
        prevEntry->setNext(entry);  // Update the next pointer of prevEntry
        }
    }

    // Recursively traverse siblings or children
    if (token->getSibling() != nullptr) {
        build(token->getSibling(), token, entry);  // Traverse sibling
    } else if (token->getChild() != nullptr) {
        pause = false;
        build(token->getChild(), token, entry);  // Traverse child
    }
}

// Checks if the token is a reserved word
bool Table::contains(std::string token){
    for (int i = 0; i < reserved.size(); i++) {
        if (token == reserved.at(i)){
            return true;
        }
    }
    return false;
}


void Table::setArray(Token* token, Entry* entry){
    entry->setArray(std::stoi(token->getSibling()->getValue()));
    build(token->getSibling()->getSibling(), token, entry); 
}


void Table::handleInitList(std::string type, Token* token, Entry* prevEntry){
    Entry *entry = new Entry(token->getValue(), "datatype", type, false, 0, scope);
    prevEntry->setNext(entry);  // Update the next pointer of prevEntry
    if (token->getSibling()->getValue() == ";") {
        return build(token->getSibling()->getChild(), token->getSibling(), entry);
    } 
    return handleInitList(type, token->getSibling()->getSibling(), entry);
}

void Table::printTable(){
    while(head != nullptr) {
        std::cout << "IDENTIFIER_NAME: " << head->getIDName() << std::endl;
        std::cout << "IDENTIFIER_TYPE: " << head->getIDType() << std::endl;
        std::cout << "DATATYPE: " << head->getDType() << std::endl;
        std::cout << "DATATYPE_IS_ARRAY: ";
        head->getIsArray() ?  std::cout << "yes" :  std::cout << "no";
        std::cout << "\nDATATYPE_ARRAY_SIZE: " << head->getArraySize() << std::endl;
        std::cout << "SCOPE: " << head->getScope() << std::endl << std::endl;
        head = head->getNext();
    }
}
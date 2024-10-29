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
                //handleParameterList(entry, token->getSibling()); // Process parameters for procedure
                return build(token->getSibling()->getSibling(), token->getSibling(), entry);
            } else {
                entry = new Entry(token->getSibling()->getValue(), prevToken->getValue(), token->getValue(), false, 0, scope);
                if (prevEntry == nullptr ){
                    head = entry;
                } else {
                    prevEntry->setNext(entry);  // Update the next pointer of prevEntry
                }
                //handleParameterList(entry, token->getSibling()); // Process parameters for the function
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

// Takes care of the parameters of a function
void Table::handleParameterList(Entry* parent, Token* startToken) {
    Token* current = startToken;

    // Ensure the starting token is a left parenthesis
    if (current->getType() != "L_PAREN") {
        std::cerr << "Error: Expected '(' but found " << current->getType() << std::endl;
        exit(1);
    }
    current = current->getSibling(); // Move to first parameter or ')' if no parameters

    // Loop to process each parameter until we reach ')'
    while (current != nullptr && current->getType() != "R_PAREN") {
        if (current->getType() == "IDENTIFIER") {
            // Create a new Entry for the parameter and set details (assuming default datatype for now)
            Entry* paramEntry = new Entry(current->getValue(), "parameter", "defaultType", false, 0, parent->getScope());
            parent->parameters.push_back(paramEntry); // Add parameter Entry to the parent Entry's parameters
        } else if (current->getType() != "COMMA") {
            std::cerr << "Error: Unexpected token in parameter list: " << current->getType() << std::endl;
            exit(1);
        }
        current = current->getSibling(); // Move to next token
    }

    // Check for closing parenthesis
    if (current == nullptr || current->getType() != "R_PAREN") {
        std::cerr << "Error: Expected ')' but not found in parameter list." << std::endl;
        exit(1);
    }
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

// Print function for any Entries that have parameters
void Table::printParameters(){

    // Cycle through all entries in the table
    while(head != nullptr) {
        if (head->parameters.size() > 0) {  // If current entry has parameters...
            std::cout << "PARAMETER LIST FOR: " << head->getIDName() << std::endl;
            for (int i = 0; i < head->parameters.size(); i++) { // Print each parameter
                std::cout << "IDENTIFIER_NAME: " << head->getIDName() << std::endl;
                std::cout << "DATATYPE: " << head->getDType() << std::endl;
                std::cout << "DATATYPE_IS_ARRAY: ";
                head->getIsArray() ? std::cout << "yes" : std::cout << "no";
                std::cout << "\nDATATYPE_ARRAY_SIZE: " << head->getArraySize() << std::endl;
                std::cout << "SCOPE: " << head->getScope() << std::endl << std::endl;
            }
        }
        head = head->getNext();
    }
}
#include <iostream>
#include "parser.h"
#include <string>
using namespace TokenTypes;
std::string currTokenType;

// Constructor that takes in our tokenList that our tokenizer created and builds a queue with it
Parser::Parser(std::vector<Token>& tokenList)
{
    this->tokenList = tokenList;
    head = &tokenList.at(0);
    for(int i = 0; i < tokenList.size(); i++){
        tokenQueue.push(&tokenList.at(i));
    }
}

// Begin state, preps queue and the pointer for being passed into state 1
void Parser::state0(){
      Token *token = tokenQueue.front();
      tokenQueue.pop();
      currTokenType = token->getType();
      state1(token);
}

// Main loop state that doesn't stop until the queue is empty
// Basically our "outer while loop" for the states
void Parser::state1(Token* lastToken) {
    while (!tokenQueue.empty()) {

        Token *token = tokenQueue.front();
        tokenQueue.pop();
        currTokenType = token->getType();

        // Checks if we have a semicolon or braces
        if (currTokenType == SEMICOLON) {
            // Checks if we need to ignore semicolons at all
            if (this->ignore > 0) {
                lastToken->setSibling(token);
                ignore--;
                state1(token);
            }
            // Otherwise go to state 2
            else {
                lastToken->setSibling(token);
                state2(token);
            }
        } else if (currTokenType == L_BRACE ) {
            lastToken->setChild(token);
            state2(token);
        } else if (lastToken->getType() == R_BRACE) {
            lastToken->setChild(token);
        } else if (lastToken->getType() == L_BRACKET) {
            state3(token);
            lastToken->setSibling(token);
        }
        lastToken->setSibling(token);    
        state1(token);
        
    }
}

void Parser::state2(Token* lastToken){
    // Base case for if the queue is empty
    if (tokenQueue.empty()) return;

    // Storing the first token in the queue
 Token *token = tokenQueue.front();

  tokenQueue.pop(); // Popping it off
    // Setting lastToken's child to be token
  lastToken->setChild(token);

    // Check for "for" loops
    if (token->getValue() == "for") {
        // Figure out a way to ignore the next two semicolons
        this->ignore += 2;
        }
  state1(token); // Go back to state 1
}

void Parser::state3(Token* token){
    
    if (token->getType() != INTEGER) {
        if (token->getType() == IDENTIFIER) {
            if (contains(token->getValue())) {
                std::cout << currTokenType;
                std::cerr << "Error on line " << token->getLineNumber() << ". Incompatbile token within square braces.";
                exit(1);
            }
        } else {
             std::cout << currTokenType;
        std::cerr << "Error on line " << token->getLineNumber() << ". Incompatbile token within square braces.";
        exit(1);
        }
    } else if (std::stoi(token->getValue()) < 0) {
        std::cerr << "Error on line " << token->getLineNumber() << ". negative value in square braces.";
    }
    return;
}

bool Parser::contains(std::string token){
    for (int i = 0; i < 5; i++) {
        if (token == reserved.at(i)){
            return true;
        }
    }
    return false;
}

void Parser::printTree(std::ofstream &rdpOutput){
    // Pointer for root of tree and counters for width/height
  Token *temp = head;
  int colCount = 0;

    // Main loop for printing the nodes
  while (temp != nullptr) {

      // Grab the first token value and print it
          std::string tokenName = temp->getValue();
          rdpOutput << tokenName;
      colCount+=tokenName.size(); // Adjust column width

          // Check if there is a sibling
          if ( temp->getSibling() != nullptr) {
              rdpOutput << "--->";
              colCount += 4; // Adjust column width
              temp = temp->getSibling();
          } else if ( temp->getChild() != nullptr) { // Check if there is a child
              rdpOutput << '\n';
                  for (int j = 1; j < colCount; j++) {
                      // Loop that prints spaces for our width
                      rdpOutput << " ";
                  }
              rdpOutput << "|\n";
              for (int k = 1; k < colCount; k++) {
                  // Loop that prints spaces for our width
                  rdpOutput << " ";
              }
              rdpOutput << "⌄\n";
              for (int i = 0; i < colCount; i++) {
                  // Loop that prints spaces for our width
                  rdpOutput << " ";
              }
              temp = temp->getChild();
          } else {
              return;
          }
      }
}

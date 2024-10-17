#include <iostream>
#include "parser.h"
std::string tokenType;

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
      tokenType = token->getType();
      state1(token);
}

// Main loop state that doesn't stop until the queue is empty
// Basically our "outer while loop" for the states
void Parser::state1(Token* lastToken){
    while (!tokenQueue.empty()) {

        Token *token = tokenQueue.front();
        tokenQueue.pop();
        tokenType = token->getType();

        // Checks if we need to set a sibling or child
        if (tokenType == SEMICOLON) {
        lastToken->setSibling(token);
            state2(token);
        } else if (tokenType == L_BRACE || tokenType == R_BRACE){
        lastToken->setChild(token);
            state2(token);
        }
        // Ask Blake about these calls, not sure why we need them
        // I think it's messing with brackets being children
        else {
            lastToken->setSibling(token);
            state1(token);
        }
    }
}

void Parser::state2(Token* lastToken){
 Token *token = tokenQueue.front();
  tokenQueue.pop();
  lastToken->setChild(token);
  state1(token);
}

void Parser::printTree(){
    // Pointer for root of tree and counters for width/height
  Token *temp = head;
  int colCount = 0;

    // Main loop for printing the nodes
  while (temp != nullptr) {

      // Grab the first token value and print it
          std::string tokenName = temp->getValue();
          std::cout << tokenName;
      colCount+=tokenName.size(); // Adjust column width

          // Check if there is a sibling
          if ( temp->getSibling() != nullptr) {
              std::cout << "--->";
              colCount += 4; // Adjust column width
              temp = temp->getSibling();
          } else if ( temp->getChild() != nullptr) { // Check if there is a child
              std::cout << '\n';
                  for (int j = 1; j < colCount; j++) {
                      // Loop that prints spaces for our width
                      std::cout << " ";
                  }
              std::cout << "|\n";
              for (int k = 1; k < colCount; k++) {
                  // Loop that prints spaces for our width
                  std::cout << " ";
              }
              std::cout << "⌄\n";
              for (int i = 0; i < colCount; i++) {
                  // Loop that prints spaces for our width
                  std::cout << " ";
              }
              temp = temp->getChild();
          } else {
              return;
          }
      }
}

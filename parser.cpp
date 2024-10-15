#include "parser.h"
std::string tokenType;
Parser::Parser(std::vector<Token>& tokenList)
{
    head = &tokenList.at(0);
    for(int i = 0; i < tokenList.size(); i++){
        tokenQueue.push(tokenList.at(i));
    }
}

void Parser::state0(){
      Token token = tokenQueue.front();
      tokenQueue.pop();
      tokenType = token.getType();
      state1(token);

}

void Parser::state1(Token lastToken){
    while (!tokenQueue.empty()) {

        Token token = tokenQueue.front();
        tokenQueue.pop();
        tokenType = token.getType();

        if (tokenType == SEMICOLON) {
        lastToken.setSibling(&token);
            state2(token);
        } else if (tokenType == L_BRACE || tokenType == R_BRACE){
        lastToken.setChild(&token);
            state2(token);
        }
        lastToken.setSibling(&token);
        state1(token);
    }
    return;
}

void Parser::state2(Token lastToken){
 Token token = tokenQueue.front();
  tokenQueue.pop();
  lastToken.setChild(&token);
  state1(token);
}

void Parser::printTree(){
  Token *temp = head;
  int rowCount = 0;
  while (temp != nullptr) {
    std::cout << temp->getValue();
    if ( temp->getSibling() != nullptr) {
      std::cout << "--------->";
      rowCount += 10;
      temp = temp->getSibling();
    } else if ( temp->getChild() != nullptr) {
      for (int i = 0; i < 10; i++){
        for (int i = 0; i < rowCount; i++){
          std::cout << " ";
        }
        std::cout << "|";
      }
      std::cout << "⌄";
      temp = temp->getChild();
    } else {
      return;
    }
  }
}

#include "parser.h"

Parser::Parser(const std::vector<Token>& tokenList)
{
    for(int i = 0; i < tokenList.size(); i++){
        tokenQueue.push(tokenList.at(i));
    }
}

void Parser::begin(){
  state0();
}

void Parser::state0(){
      while (!tokenQueue.empty()) {

        Token token = tokenQueue.front();
        tokenQueue.pop();
        tokenType = token.getType();

        if (tokenType == IDENTIFIER) {
            state1(token);
        } else if (tokenType == INTEGER) {

        } else if (tokenType == DOUBLE_QUOTE) {

        } else if (tokenType == SINGLE_QUOTE) {

        } else if (tokenType == COMMA) {

        } else if (tokenType == SEMICOLON) {

        } else if (tokenType == ASSIGNMENT_OPERATOR) {

        } else if (tokenType == L_BRACE) {

        } else if (tokenType == R_BRACE) {

        } else if (tokenType == L_BRACKET) {

        } else if (tokenType == R_BRACKET) {

        } else if (tokenType == STRING) {

        } else if (tokenType == PLUS) {

        } else if (tokenType == MINUS) {

        } else if (tokenType == LT_EQUAL) {

        } else if (tokenType == GT_EQUAL) {

        } else if (tokenType == GT) {

        } else if (tokenType == LT) {

        } else if (tokenType == BOOLEAN_EQUAL) {

        } else if (tokenType == BOOLEAN_AND) {

        } else if (tokenType == ASTERISK) {

        } else if (tokenType == CHARACTER) {

        }
    }
    return;
}

void Parser::state1(Token lastToken){
    Token token = tokenQueue.front();
    tokenQueue.pop();
    tokenType = token.getType();

    if (tokenType == IDENTIFIER) {
        lastToken.getSibling() = token;       
    } 
}
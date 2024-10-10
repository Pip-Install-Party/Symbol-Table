//**************
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include "tokenizer.h"

// Base state. Branch elsewhere from here.
// This state processes characters, numbers, and punctuation, directing control to specific states based on input.
void Tokenizer::state0(std::istringstream &inputStream, int &lineCount) {
    char ch;
    
    while (true) {
        inputStream.get(ch);  // Get the next character from the input stream.
        
        if (inputStream.eof()) {  // End of input, stop processing.
            return;
        } else if (ch == '\n') {  // Handle newlines by incrementing the line count.
            lineCount++;
        } else if (isspace(ch)) {  // Skip all whitespace characters (space, tab, etc.).
            continue;
        } else if (ch == '(') {  // Handle left parentheses.
            Token token("L_PAREN", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (ch == ')') {  // Handle right parentheses.
            Token token("R_PAREN", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (ch == '{') {  // Handle left braces.
            Token token("L_BRACE", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (ch == '}') {  // Handle right braces.
            Token token("R_BRACE", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (ch == '[') {  // Handle left brackets.
            Token token("L_BRACKET", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (ch == ']') {  // Handle right brackets.
            Token token("R_BRACKET", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (ch == ';') {  // Handle semicolons.
            Token token("SEMICOLON", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (ch == '/') {  // Handle division operator.
            Token token("DIVIDE", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (isdigit(ch)) {  // Handle integer literals.
            inputStream.putback(ch);  // Put character back for further processing.
            
            std::string intLiteral;
            state12(inputStream, lineCount, intLiteral);  // Transition to state12 to process the number.

            Token token("INTEGER", intLiteral, lineCount);
            tokenList.push_back(token);
        } else if (ch == ',') {  // Handle commas.
            Token token("COMMA", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (ch == '=') {  // Handle assignment or equality.
            state13(inputStream, lineCount);  // Transition to state13 for further processing.
        } else if (ch == '+') {  // Handle plus operator.
            Token token("PLUS", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (ch == '-') {  // Handle minus sign or negative integers.
            state2(inputStream, lineCount);
        } else if (ch == '>') {  // Handle greater than or greater than or equal to operator.
            state6(inputStream, lineCount);
        } else if (ch == '<') {  // Handle less than or less than or equal to operator.
            state7(inputStream, lineCount);
        } else if (ch == '*') {  // Handle asterisk operator.
            Token token("ASTERISK", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (ch == '%') {  // Handle modulo operator.
            Token token("MODULO", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (ch == '\\') {  // Handle backslash character.
            Token token("BACKSLASH", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (ch == '&') {  // Handle boolean AND or bitwise AND.
            state10(inputStream, lineCount);
        } else if (ch == '"') {  // Handle double quotes (beginning of a string literal).
            Token token = Token("DOUBLE_QUOTE", std::string(1, ch), lineCount);
            tokenList.push_back(token);
            std::string strLiteral;
            state1(inputStream, lineCount, strLiteral);  // Transition to state1 to handle the string.
        } else if (ch == '\'') {  // Handle single quotes (character literals).
            Token token("SINGLE_QUOTE", std::string(1, ch), lineCount);
            tokenList.push_back(token);
            state5(inputStream, lineCount);  // Transition to state5 for single quotes.
        } else if (ch == '_') {  // Handle underscores.
            Token token("UNDERSCORE", std::string(1, ch), lineCount);
            tokenList.push_back(token);
        } else if (isalpha(ch) || ch == '_') {  // Handle identifiers (letters or underscores).
            std::string identifier(1, ch);

            while (inputStream.get(ch) && (isalnum(ch) || ch == '_')){
                identifier += ch;
            }
            inputStream.putback(ch);

            Token token("IDENTIFIER", identifier, lineCount);
            tokenList.push_back(token);
        } else {
            // If an unrecognized token is encountered, output an error message and terminate.
            state9(std::to_string(ch),"Unrecognized token: ", "at line" + std::to_string(lineCount) + " - Terminating...\n");
        }
    }
}

// Handle string literals enclosed by double quotes
void Tokenizer::state1(std::istringstream &inputStream, int &lineCount, std::string &strLiteral) {
    char ch;
    inputStream.get(ch);
    
    if (inputStream.eof()) {  // Handle the end of the file.
        state9(std::to_string(lineCount),"Error: Unterminated string on line ", "");
    } else if (ch == '"') {  // End of the string literal.
        Token token = Token("STRING", strLiteral, lineCount);
        tokenList.push_back(token);

        Token closingToken("DOUBLE_QUOTE", std::string(1, ch), lineCount);
        tokenList.push_back(closingToken);
        return;
    } else if (ch == '\n') {  // Handle newlines within the string (error).
        lineCount++;
        state9(std::to_string(lineCount),"Error: Unterminated string on line ", "");
    } else {  // Add characters to the buffer as part of the string.
        strLiteral += ch;
    }
    state1(inputStream, lineCount, strLiteral);  // Continue processing the string.
}

// Check if - is part of a number or just a minus sign.
void Tokenizer::state2(std::istringstream &inputStream, int &lineCount) {
    char ch;
    inputStream.get(ch);  // Get the next character.

    if (inputStream.eof()) {  // End of file, return error for unterminated hyphen.
        state9("","Error: Unterminated hyphen\n", "");
    } else if (ch == '\n') {  // Handle newline, increment line count.
        lineCount++;
    } else if (isdigit(ch)) {  // Handle negative numbers.
        std::string negativeInt = "-" + std::string(1,ch);
        std::cout << "moving to state 3 from state 2, negativeInt is: " << negativeInt << "\n";
        state3(inputStream, lineCount, negativeInt);  // Transition to state3 for further number processing.
        return;
    } else {
        inputStream.putback(ch);  // If it's not part of a number, put the character back.
    }
    Token token("MINUS", "-", lineCount);
    tokenList.push_back(token);
    return;
}

// Process integer tokens.
void Tokenizer::state3(std::istringstream &inputStream, int &lineCount, std::string &negativeInt) {
    char ch;
    inputStream.get(ch);  // Get the next character.
    
    if (isdigit(ch)) {  // Keep accumulating digits.
        negativeInt += ch;
        state3(inputStream, lineCount, negativeInt);  // Continue processing.
        return;
    } else {  // Case when a non-digit is read.
        inputStream.putback(ch);  // Put back the non-digit character.

        // Add the complete negative integer to the token list.
        Token token("INTEGER", negativeInt, lineCount);
        tokenList.push_back(token);

        return;  // Return to the previous state
    }
}

// Process identifier tokens (letters and underscores).
void Tokenizer::state4(std::istringstream &inputStream, int &lineCount, std::string &identifier) {
    char ch;

    while (true) {
        inputStream.get(ch);  // Get the next character.

        if (inputStream.eof()) {  // End of file, handle error for unterminated identifier.
            state9("","Error: Unterminated identifier\n", "");
        } else if (!isalnum(ch) && ch != '_') {  // If it's not a valid identifier character, stop.
            inputStream.putback(ch);  // Put the character back.
            break;
        }

        identifier += ch;  // Add character to the identifier token.
        }
}

// Handle characters enclosed by single quotes.
void Tokenizer::state5(std::istringstream &inputStream, int &lineCount) {
    char ch;
    inputStream.get(ch);  // Get the next character.

    if (inputStream.eof()) {  // End of file, error for unterminated character literal.
        state9(std::to_string(lineCount),"Error: Unterminated character literal on line", "");
    } else if (ch == '\'') {  // Empty character literal, handle it.
        Token token("SINGLE_QUOTE", std::string(1, ch), lineCount);
        tokenList.push_back(token);
        return;
    } else if (ch == '\\') {  // Escape character inside the literal.
        inputStream.get(ch); // Found backslash so need to get escaped character as well
        Token token("CHARACTER", std::string(1, ch), lineCount);
        tokenList.push_back(token);
    } else {  // Handle regular single length character literals.
        Token token("CHARACTER", std::string(1, ch), lineCount);
        tokenList.push_back(token);

    }
    state11(inputStream, lineCount);  // Check for the closing single quote.
}

// Process greater-than operator and greater-than-or-equal-to operator.
void Tokenizer::state6(std::istringstream &inputStream, int &lineCount) {
    char ch;
    inputStream.get(ch);  // Get the next character.

    if (ch == '=') {  // Handle '>=' (greater than or equal to).
        Token token("GT_EQUAL", std::string(1, ch), lineCount);
        tokenList.push_back(token);
    } else {  // Handle '>' (greater than).
        inputStream.putback(ch);  // Put character back if not '='.
        Token token("GT", ">", lineCount);
        tokenList.push_back(token);
    }
    return;
}

// Process less-than operator and less-than-or-equal-to operator.
void Tokenizer::state7(std::istringstream &inputStream, int &lineCount) {
    char ch;
    inputStream.get(ch);  // Get the next character.

    if (ch == '=') {  // Handle '<=' (less than or equal to).
        Token token("LT_EQUAL", std::string(1, ch), lineCount);
        tokenList.push_back(token);
    } else {  // Handle '<' (less than).
        inputStream.putback(ch);  // Put character back if not '='.
        Token token("LT", "<", lineCount);
        tokenList.push_back(token);
    }
    return;
}

// Handle escape sequences inside string literals (e.g., '\n', '\t') and character literals 
void Tokenizer::state8(std::istringstream &inputStream, int &lineCount) {

    char ch;
    inputStream.get(ch);  // Get the character after the backslash.

    // Handle valid escape characters like '\n', '\t', etc.
    if (ch == 'n' || ch == 't' || ch == '\\'  || ch == '0' || ch == '\'' || ch == '\\' 
                || ch == 'r' || ch == 'b' || ch == 'f'|| ch == 'a'|| ch == 'v') {
        Token token("CHARACTER", std::string(1, ch), lineCount);
        tokenList.push_back(token);
    } else {  // Handle invalid escape sequences.
        state9(std::to_string(ch), "Error: Invalid escape sequence '\\", "");
    }
    return; 
}

// Dead State
void Tokenizer::state9(std::string var, std::string message, std::string message2) {
    std::cerr << message << var << message2;
    exit(1);
}

// Handle boolean AND (&&) and bitwise AND (&).
void Tokenizer::state10(std::istringstream &inputStream, int &lineCount) {
    char ch;
    inputStream.get(ch);  // Get the next character.

    if (ch == '&') {  // Handle boolean AND (&&).
        Token token("BOOLEAN_AND", std::string(1, ch), lineCount);
        tokenList.push_back(token);
    } else {  // Handle bitwise AND (&).
        inputStream.putback(ch);  // Put character back if not another '&'.
        Token token("AMPERSAND", std::string(1, ch), lineCount);
        tokenList.push_back(token);
    }
}

// Check for the closing single quote for character literals.
void Tokenizer::state11(std::istringstream &inputStream, int &lineCount) {
    char ch;
    inputStream.get(ch);  // Get the next character.

    if (ch == '\'') {  // Handle closing single quote.
        Token token("SINGLE_QUOTE", std::string(1, ch), lineCount);
        tokenList.push_back(token);
        return;
    } else {  // Handle invalid character literals.
        state9(std::to_string(lineCount), "Error: Invalid character literal on line ", "");
    }
}

// Process sequences of digits (integer values).
void Tokenizer::state12(std::istringstream &inputStream, int &lineCount, std::string &intLiteral) {
    char ch;
    inputStream.get(ch);  // Get the next character.

    // Keep reading digits until a non-digit character is found.
    if (!isdigit(ch)) {
        inputStream.putback(ch);  // Put back the non-digit character.
        state14(inputStream, lineCount);  // Transition to state14 to validate.
        return;
    }
    intLiteral += ch;
    state12(inputStream, lineCount, intLiteral);  // Continue processing digits.
}

// Process assignment '=' and equality '=='.
void Tokenizer::state13(std::istringstream &inputStream, int &lineCount) {
    char ch;
    inputStream.get(ch);  // Get the next character.

    if (ch == '=') {  // Handle equality '=='.
        Token token("BOOLEAN_EQUAL", "==", lineCount);
        tokenList.push_back(token);
    } else {  // Handle assignment '='.
        inputStream.putback(ch);  // Put back the character if it's not another '='.
        Token token("ASSIGNMENT_OPERATOR", "=", lineCount);
        tokenList.push_back(token); 
        
    }
    return;
}

// Check and validate the termination of integer tokens.
void Tokenizer::state14(std::istringstream &inputStream, int &lineCount) {
    char ch;
    inputStream.get(ch);  // Get the next character.

    // Ensure that the number is followed by a valid token terminator (space, semicolon, closing parenthesis, or bracket).
    if (ch != ' ' && ch != ';' && ch != ')' && ch != ']') {
        state9(std::to_string(lineCount),"Syntax error on line ", ": invalid integer\n");
    }
    // If it's a valid token terminator, put the character back into the stream for further processing.
    inputStream.putback(ch);
}
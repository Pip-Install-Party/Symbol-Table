#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream> 

#include "commentDFA.h"
#include "tokenizer.h"

// Vectors holding the file paths for test files
const std::filesystem::path a1Tests[] = {
   "Tests/Program1/programming_assignment_1-test_file_1.c",
   "Tests/Program1/programming_assignment_1-test_file_2.c",
   "Tests/Program1/programming_assignment_1-test_file_3.c",
   "Tests/Program1/programming_assignment_1-test_file_4.c",
   "Tests/Program1/programming_assignment_1-test_file_5.c",
   "Tests/Program1/programming_assignment_1-test_file_6.c"
};

const std::filesystem::path a2Tests[] = {
   "Tests/Program2/programming_assignment_2-test_file_1.c",
   "Tests/Program2/programming_assignment_2-test_file_2.c",
   "Tests/Program2/programming_assignment_2-test_file_3.c",
   "Tests/Program2/programming_assignment_2-test_file_4.c",
   "Tests/Program2/programming_assignment_2-test_file_5.c",
   "Tests/Program2/programming_assignment_2-test_file_6.c"
};




// Main program function that handles user prompt, file opening/closing, and initial state
int main() {
    char ch;
    int assignment_num;
    int filenum;
    std::ifstream file;
    std::ostringstream buffer;

    // Prompt user for what programming assignment they want to test
    std::cout << "Which programming assignment would you like to test?\n\n"
         "(1) Programming Assignment 1: Ignore Comments\n"
         "(2) Programming Assignment 2: Tokenization\n"
         "Selection: ";
    std::cin >> assignment_num;

    std::cout << "Which file would you like to remove comments from?\n\n";

    if (assignment_num == 1) {
        for (int i = 0; i < std::size(a1Tests); i++) {
            std::cout << "(" << i+1 << ")" << a1Tests[i] << std::endl;
        }
    } else if (assignment_num == 2) {
         for (int i = 0; i < std::size(a2Tests); i++) {
            std::cout << "(" << i+1 << ")" << a2Tests[i] << std::endl;
        }
    }
    std::cout << "Selection: ";
    std::cin >> filenum;
            
    filenum -= 1;

    if (assignment_num == 1) {
        file.open(a1Tests[filenum]);
        if (!file.is_open()) {  // Check if the file was opened successfully
            std::cerr << "Error: Could not open the file " << a1Tests[filenum] << std::endl;
            exit(1);
        }
    } else if (assignment_num == 2) {
        file.open(a2Tests[filenum]);
        if (!file.is_open()) {  // Check if the file was opened successfully
            std::cerr << "Error: Could not open the file " << a2Tests[filenum] << std::endl;
            exit(1);
         }
    }  else {
        std::cerr << "Inavlid file selection... Exiting...";
        exit(1);
    }
    CommentDFA *removeComments = new CommentDFA();
    // Start in state0 to process the file
    removeComments->begin(file, buffer);
    file.close();  // Close the file after processing

    if (assignment_num == 1) {
        std::cout  << "\nResulting File:" << std::endl;
        std::cout << buffer.str();
    }
    else if(assignment_num == 2) {
        std::string str = buffer.str();
        str.erase(str.find_last_not_of(" \n\r\t")+1);  // Remove trailing whitespace or newlines
        std::istringstream inputStream(str);
        
        Tokenizer *tokenizer = new Tokenizer();

        tokenizer->begin(inputStream);

        std::vector<Token> tokenList = tokenizer->getTokens();

        std::cout << "Token List\n";

        for (int i = 0; i < tokenList.size(); i++){
            std::cout << "Token type: " << tokenList[i].tokenType << '\n';
            std::cout << "Token: " << tokenList[i].token << "\nLine Number: " << tokenList[i].lineNumber << '\n';
            std::cout << "\n";
        }
    }
    else if(assignment_num == 3) {
        std::cout << "Assignment 3, not implemented yet\n";
    }
    else {
        std::cout << "No matching assignments exist for " << assignment_num << ".\n";
        return 1;
    }
    

    return 0;  // Exit successfully
}
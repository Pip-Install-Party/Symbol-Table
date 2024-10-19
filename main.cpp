#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>

#include "commentDFA.h"
#include "tokenizer.h"
#include "parser.h"
#include "table.h"

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

const std::filesystem::path a3Tests[] = {
    "Tests/Program3/programming_assignment_3-test_file_1.c",
    "Tests/Program3/programming_assignment_3-test_file_2.c",
    "Tests/Program3/programming_assignment_3-test_file_3.c",
    "Tests/Program3/programming_assignment_3-test_file_4.c",
    "Tests/Program3/programming_assignment_3-test_file_5.c",
    "Tests/Program3/programming_assignment_3-test_file_6.c",
    "Tests/Program3/programming_assignment_3-test_file_7.c",
    "Tests/Program3/programming_assignment_3-test_file_8.c",
    "Tests/Program3/programming_assignment_3-test_file_9.c",
    "Tests/Program3/programming_assignment_3-test_file_10.c"
};

const std::filesystem::path a4Tests[] = {
    "Tests/Program4/programming_assignment_4-test_file_1.c",
    "Tests/Program4/programming_assignment_4-test_file_2.c",
    "Tests/Program4/programming_assignment_4-test_file_3.c",
    "Tests/Program4/programming_assignment_4-test_file_4.c",
    "Tests/Program4/programming_assignment_4-test_file_5.c",
    "Tests/Program4/programming_assignment_4-test_file_6.c",
    "Tests/Program4/programming_assignment_4-test_file_7.c"
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
         "(3) Programming Assignment 3: Recursive Descent Parser\n"
         "(4) Programming Assignment 4: Symbol Table\n"
         "Selection: ";
    std::cin >> assignment_num;

    std::cout << "Which file would you like to run the program on?\n\n";

    if (assignment_num == 1) {
        for (int i = 0; i < std::size(a1Tests); i++) {
            std::cout << "(" << i+1 << ")" << a1Tests[i] << std::endl;
        }
    } else if (assignment_num == 2) {
         for (int i = 0; i < std::size(a2Tests); i++) {
            std::cout << "(" << i+1 << ")" << a2Tests[i] << std::endl;
        }
    } else if (assignment_num == 3) {
         for (int i = 0; i < std::size(a3Tests); i++) {
            std::cout << "(" << i+1 << ")" << a3Tests[i] << std::endl;
        }
    } else if (assignment_num == 4) {
         for (int i = 0; i < std::size(a4Tests); i++) {
            std::cout << "(" << i+1 << ")" << a4Tests[i] << std::endl;
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
    }   else if (assignment_num == 3) {
        file.open(a3Tests[filenum]);
        if (!file.is_open()) {  // Check if the file was opened successfully
            std::cerr << "Error: Could not open the file " << a3Tests[filenum] << std::endl;
            exit(1);
         }
    } else if (assignment_num == 4) {
        file.open(a4Tests[filenum]);
        if (!file.is_open()) {  // Check if the file was opened successfully
            std::cerr << "Error: Could not open the file " << a4Tests[filenum] << std::endl;
            exit(1);
         }
    } else {
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
            std::cout << "Token type: " << tokenList[i].getType() << '\n';
            std::cout << "Token: " << tokenList[i].getValue() << "\nLine Number: " << tokenList[i].getLineNumber() << '\n';
            std::cout << "\n";
        }
    }
    else if(assignment_num == 3) {
         std::string str = buffer.str();
        str.erase(str.find_last_not_of(" \n\r\t")+1);  // Remove trailing whitespace or newlines
        std::istringstream inputStream(str);

        Tokenizer *tokenizer = new Tokenizer();

        tokenizer->begin(inputStream);

        std::vector<Token> tokenList = tokenizer->getTokens();

        Parser *parser = new Parser(tokenList);
        parser->begin();

        // Make an output filestream
        std::ofstream rdpOutput( "test_file_" + std::to_string(filenum + 1) + "_output.txt");

        // Call the print function and pass in the ofstream
        parser->printTree(rdpOutput);
    } else if(assignment_num == 4) {
        std::string str = buffer.str();
        str.erase(str.find_last_not_of(" \n\r\t")+1);  // Remove trailing whitespace or newlines
        std::istringstream inputStream(str);

        Tokenizer *tokenizer = new Tokenizer();

        tokenizer->begin(inputStream);

        std::vector<Token> tokenList = tokenizer->getTokens();

        Parser *parser = new Parser(tokenList);
        parser->begin();

        // Make an output filestream
        std::ofstream rdpOutput( "st_test_file_" + std::to_string(filenum + 1) + "_output.txt");

        Table *table = new Table;

        table->begin(parser->getHead());
        table->printTable();
    }
    else {
        std::cout << "No matching assignments exist for " << assignment_num << ".\n";
        return 1;
    }

    return 0;  // Exit successfully
}
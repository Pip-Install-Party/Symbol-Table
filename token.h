// The Token class defines the attributes of a Token, including its type, contents, and line number.
class Token 
{
private: 
    Token *lChild;
    Token *rSibling;
    std::string tokenType = "";
    std::string token = "";
    int lineNumber = 0;
public:

    Token(const std::string& tokenType, const std::string& token, int lineNumber)
        : tokenType(tokenType), token(token), lineNumber(lineNumber) {}

    // Get the type of the token.
    std::string getType() const {
        return tokenType;
    }

    // Get the value of the token.
    std::string getValue() const {
        return token;
    }

    // Get the line number where the token was found
    int getLineNumber() const {
        return lineNumber;
    }

    Token getSibling(){ return *rSibling; }
    Token getChild(){ return *lChild; }
};
#ifndef ENTRY_H
#define ENTRY_H

#include <string>

class Entry {
    private:
    std::string idName;
    std::string idType;
    std::string dType;
    bool isArray;
    int arraySize;
    short scope;

    Entry* nextEntry; 
    public:
    Entry(std::string idName, std::string idType, std::string dType, bool isArray, int arraySize, short scope)
        : idName(idName), idType(idType), dType(dType), isArray(isArray), arraySize(arraySize), scope(scope){}; 
    
    void setNext(Entry* entry) { nextEntry = entry; }
    std::string getIDName(){ return idName; }
    std::string getIDType(){ return idType; }
    std::string getDType() { return dType; }
    bool getIsArray() { return isArray; }
    int getArraySize() { return arraySize; }
    short getScope() { return scope; }
    Entry* getNext() { return nextEntry; }
    void setArray(int size){ isArray = true; arraySize = size; }
};


#endif // ENTRY_H
#include "FrequencyCodes.h"
#include <iostream>
#include "HuffmanTree.h"
#include <map>
#include <ostream>
#include <string>
void generateCodes(Node* rootNode, std::string currentCode, std::map <char, std::string> &currentCodes) {
    if (rootNode->left == nullptr && rootNode->right == nullptr) {
        currentCodes[rootNode->character] = currentCode;
        return;
    }
        generateCodes(rootNode->left, currentCode + "0", currentCodes);
        generateCodes(rootNode->right, currentCode + "1", currentCodes);
}
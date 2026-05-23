#include "Decompressor.h"
#include "HuffmanTree.h"
#include <bitset>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <filesystem>
void decompressFile(std::ifstream& compressedFile, std::string filePath) {
    if (filePath.find("(compressed)") != std::string::npos) {
        filePath.erase(filePath.find("(compressed)"), 12);
    }
    std::filesystem::path decompressedFilePath = filePath;
    char firstChar = compressedFile.get();
    char c;
    std::string originalFileExtension = "";
    for (int i = 0; i < static_cast<int>(firstChar); i++) {
        compressedFile.get(c);
        originalFileExtension+=c;
    }
    decompressedFilePath.replace_extension(originalFileExtension);
    std::ofstream restoredFile(decompressedFilePath, std::ios::binary);
    int uniqueCharCount;
    compressedFile.read(reinterpret_cast<char*>(&uniqueCharCount), sizeof(int));
    std::vector<int>restoredFrequency(256, 0);
    for (int i = 0; i < uniqueCharCount; i++) {
        unsigned char charType;
        int charFrequency;
        compressedFile.read(reinterpret_cast<char*>(&charType), sizeof(char));
        compressedFile.read(reinterpret_cast<char*>(&charFrequency), sizeof(int));
        restoredFrequency[static_cast<int>(charType)] = charFrequency;
    }
    int totalCharacters;
    compressedFile.read(reinterpret_cast<char*>(&totalCharacters), sizeof(int));
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (int i = 0; i < restoredFrequency.size(); i++) {
        if (restoredFrequency.at(i) > 0) {
            Node *node = new Node(static_cast<char>(i), restoredFrequency.at(i), nullptr, nullptr);
            pq.push(node);
        }
    }
    while (pq.size() > 1) {
        Node *nodeRight = pq.top();
        pq.pop();
        Node *nodeLeft = pq.top();
        pq.pop();
        Node *node3 = new Node(nodeRight->frequency + nodeLeft->frequency, nodeLeft, nodeRight);
        pq.push(node3);
    }
    char ch;
    Node *currentNode = pq.top();
    while (compressedFile.get(ch) && totalCharacters > 0) {
        std::bitset<8> byte(ch);
        std::string byteString = byte.to_string();
        for (char c : byteString) {
            if (c == '1') {
                currentNode = currentNode->right;
            }
            else {
                currentNode = currentNode->left;
            }
            if (currentNode->left == nullptr && currentNode->right == nullptr) {
                restoredFile.put(currentNode->character);
                currentNode = pq.top();
                totalCharacters--;
            }
        }
    }
    delete pq.top();
    restoredFile.close();
}
#include "BitStreamWriter.h"
#include <fstream>
#include <map>
#include <filesystem>
void writeHeader(std::ofstream &compressedFile, std::string fileExtension,
    std::vector<int> frequency, int chCount, int totalCharacters) {
    compressedFile.put(static_cast<char>(fileExtension.length()));
    compressedFile.write(fileExtension.c_str(), fileExtension.length() * sizeof(char));
    compressedFile.write(reinterpret_cast<char*>(&chCount), sizeof(int));
    for (int i = 0; i < frequency.size(); i++) {
        if (frequency.at(i) > 0) {
            unsigned char asciiValue = static_cast<unsigned char>(i);
            compressedFile.put(asciiValue);
            compressedFile.write(reinterpret_cast<char*>(&frequency.at(i)), sizeof(int));
        }
    }
    compressedFile.write(reinterpret_cast<char*>(&totalCharacters), sizeof(int));
}
void writeCompressedFile(std::ifstream *inputFile, std::string filePath,
    std::map<char, std::string> &codes, std::string fileExtension,
    std::vector<int> frequency, int size, int chCount, int totalCharacters) {
    std::ofstream compressedFile(filePath, std::ios::binary);
    writeHeader(compressedFile, fileExtension, frequency, chCount, totalCharacters);
    char character;
    unsigned char buffer = '\0';
    int bitCount = 0;
    while (inputFile->get(character)) {
        std::string code = codes[character];
        for (char c : code) {
            buffer <<= 1;
                if (c == '1') {
                    buffer |= 1;
                }
            bitCount++;
            if (bitCount == 8) {
                compressedFile.write(reinterpret_cast<char*>(&buffer), 1);
                buffer = '\0';
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0) {
        buffer <<= 8 - bitCount;
        compressedFile.write(reinterpret_cast<char*>(&buffer), 1);
    }
    compressedFile.close();
}
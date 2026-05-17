#include "BitStreamWriter.h"
#include <fstream>
#include <map>
#include <filesystem>
void writeCompressedFile(std::ifstream *inputFile, std::string filePath, std::map<char, std::string> &codes) {
    std:: ofstream compressedFile(filePath, std::ios::binary);
    char ch;
    unsigned char buffer = '\0';
    int bitCount = 0;
    while (inputFile->get(ch)) {
        std::string code = codes[ch];
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
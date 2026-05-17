#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "HuffmanTree.h"
#include "FrequencyCodes.h"
#include "BitStreamWriter.h"
#include <map>

int main() {
    int frequency[256] = {0};
    std::ifstream file("C:\\Users\\tonyw\\Downloads\\TicTacToeProject\\stats.txt", std::ios_base::binary);
    std::string compressed = "(compressed)";
    std::cout << "Enter the path to the file: ";
    std::filesystem::path originalFilePath = "C:\\Users\\tonyw\\Downloads\\TicTacToeProject\\stats.txt";
    std::cin >> originalFilePath;
    std::filesystem::path compressedFilePath = originalFilePath.parent_path().string() + "\\" + originalFilePath.stem().string() + compressed;
    compressedFilePath.replace_extension(".huff");
    char ch;
    std::map<char, std::string> frequencyCodes;
    if (file.is_open()) {
        while (file.get(ch)) {
            frequency[static_cast<unsigned char>(ch)]++;
        }
    }
    std::ifstream inputFile("C:\\Users\\tonyw\\Downloads\\TicTacToeProject\\stats.txt", std::ios_base::binary);
    generateCodes(buildHuffmanTree(frequency, 256), "", frequencyCodes);
    writeCompressedFile(&inputFile, compressedFilePath.string(),frequencyCodes);
    std::cout << "File Size: "<< std::filesystem::file_size(originalFilePath) << " bytes" << std::endl;
    std::cout << "Compressed File Size: " << std::filesystem::file_size(compressedFilePath) << " bytes" << std::endl;
    std::cout << "That is a " << 100.0 - (static_cast<double>(std::filesystem::file_size(compressedFilePath)) /
        static_cast<double>(std::filesystem::file_size(originalFilePath)) * 100) << "% decrease!" <<std::endl;
    file.close();
    return 0;
}
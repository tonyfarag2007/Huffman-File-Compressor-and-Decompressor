#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include "HuffmanTree.h"
#include "FrequencyCodes.h"
#include "BitStreamWriter.h"
#include "Decompressor.h"
#include <map>

int main() {
    std::vector<int> frequency(256, 0);
    char choice;
    std::cout << "(C)ompress or (D)ecompress?"<<std::endl;
    std::cin >> choice;
    if (choice == 'D' || choice == 'd') {
        std::cout<<"Enter the path to the compressed .huff file" <<std::endl;
        std::string compressedFilePath;
        std::cin >> compressedFilePath;
        std::ifstream file(compressedFilePath, std::ios_base::binary);
        if (!file.is_open()) {
            std::cout << "Failed to open: " << compressedFilePath << std::endl;
            exit(1);
        }
        decompressFile(file);
        exit(0);
    }
    else {
        std::filesystem::path originalFilePath;
        std::cout << "Enter the path to the file: ";
        std::string buffer;
        std::cin.ignore();
        std::getline(std::cin, buffer);
        originalFilePath = std::filesystem::path(buffer);
        std::string fileExtension = originalFilePath.extension().string();
        std::ifstream file(originalFilePath, std::ios_base::binary);
        std::filesystem::path compressedFilePath = originalFilePath.parent_path().string() + "\\" + originalFilePath.stem().string() + "(compressed)";
        compressedFilePath.replace_extension(".huff");
        char ch;
        int chCount = 0;
        std::map<char, std::string> frequencyCodes;
        if (file.is_open()) {
            while (file.get(ch)) {
                frequency[static_cast<unsigned char>(ch)]++;
            }
        }
        else {
            std::cout << "CRITICAL: Could not open file: " << originalFilePath << std::endl;
        }
        int totalCharacters = 0;
        for (int i = 0; i < 256; i++) {
            if (frequency[i] > 0) chCount++;
            totalCharacters += frequency[i];
        }
        file.clear();
        file.seekg(0, std::ios::beg);
        Node *rootNode = buildHuffmanTree(frequency, 256);
        generateCodes(rootNode, "", frequencyCodes);
        delete rootNode;
        writeCompressedFile(&file, compressedFilePath.string(),frequencyCodes,
            originalFilePath.extension().string(), frequency, 256, chCount, totalCharacters);
        std::cout << "File Size: "<< std::filesystem::file_size(originalFilePath) << " bytes" << std::endl;
        std::cout << "Compressed File Size: " << std::filesystem::file_size(compressedFilePath) << " bytes" << std::endl;
        std::cout << "That is a " << 100.0 - (static_cast<double>(std::filesystem::file_size(compressedFilePath)) /
            static_cast<double>(std::filesystem::file_size(originalFilePath)) * 100) << "% decrease!" <<std::endl;
        file.close();
    }
    return 0;
}
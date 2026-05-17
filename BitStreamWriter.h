#ifndef HUFFMAN_BITSTREAMWRITER_H
#define HUFFMAN_BITSTREAMWRITER_H
#include <fstream>
#include <map>
#include <string>
void writeCompressedFile(std:: ifstream*, std::string, std::map<char, std::string>&);
#endif //HUFFMAN_BITSTREAMWRITER_H
/*
 *  HuffmanCoder.h
 *  Harry Lynch
 *  11/7/2024
 *
 *  Interface for the main driver class for both the Huffman encoding and
 *  decoding processes. This includes all the proceses for creating a huffman
 *  tree given a set of inputs from counting frequencies to building the binary
 *  file on the encoding side. On the decoding side, this file covers the whole
 *  process from deserialization to re-conversion
 *
 */

#ifndef _HUFFMAN_CODER
#define _HUFFMAN_CODER

#include "HuffmanTreeNode.h"
#include "ZapUtil.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
class HuffmanCoder {
    // Feel free to add additional private helper functions as well as a
    // constructor and destructor if necessary
  public:
    HuffmanCoder();
    ~HuffmanCoder();
    void encoder(const std::string &inputFile, const std::string &outputFile);
    void decoder(const std::string &inputFile, const std::string &outputFile);

  private:
    HuffmanTreeNode *root;
    std::unordered_map<char, int> freqMap;
    std::unordered_map<char, std::string> codeMap;
    std::ifstream inputStream;
    std::ofstream outputStream;

    void buildTree();
    void createCharCodes();
    void charCodeHelper(HuffmanTreeNode *curr, std::string &currCode,
                        char leftOrRight);
    std::string asciiToBinary(const string &inputFile);
    void count_freqs(std::istream &text);
    std::string serialize_tree(HuffmanTreeNode *curr);

    void deserialize_tree(const std::string &s);
    HuffmanTreeNode *deserialize_helper(const std::string &s, int &index);
    std::string binaryToASCII(std::string &binString);

    void cleanTree(HuffmanTreeNode *huffRoot);
    void debug_printMaps(std::string map);
    template <typename streamtype>
    void open_or_die(streamtype &stream, std::string fileName);
};

#endif
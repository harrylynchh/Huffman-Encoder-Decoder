
Huffman Encoder/Decoder -- Lossless Compression
Harry Lynch  11/7/2024

**Summary:**
-------------------------

This program, Zap, is an implementation of Huffman Coding compression which
takes a tree of weighted characters to encode ASCII chars into a binary string.
This binary string in combination with a serialized version of the Huffman Tree
used to encode the characters is compiled into binary. This binary file can also
be decoded by deserializing the tree from the binary. Then, this tree can be
used as a reference to re-create the original ASCII text.

**Compilation/Usage:**
-------------------------

To compile the program, use the 'make' command. 
Usage:

    ./zap [zap | unzap] inputFile outputFile

To encode a file, use [zap] with a .txt input file and a .zap outputfile Ex:

    ./zap zap words.txt encoded.zap

This will encode the ASCII text in words.txt into binary, compressing it.  To
decode and retrieve the original text in a lossless fashion, execute:

    ./zap unzap encoded.zap finalText.txt

Where the .zap file will be decoded and turned into ASCII once again.

**Files Provided:**
-------------------------

main.cpp - holds main function

HuffmanCoder.cpp - primary driver function

HuffmanCoder.h - implementation of driver fn 

HuffmanTreeNode.h - implementation of nodes used for HuffTree

ZapUtil.h - util to read and write to binary files 

Makefile - rules to 'make'

README - documentation

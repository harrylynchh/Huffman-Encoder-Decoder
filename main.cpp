/*
 *  main.cpp
 *  Harry Lynch
 *  11/7/2024
 *
 *  Starting function for Zap.  Manages cmd parameters and builds the
 *  HuffmanCoder and parses from the input params whether or not to encode or
 *  decode. Transfers user input into the program and exits the program aswell
 *  (implicitly calls HuffmanCoder deconstructor).
 *
 */

#include "HuffmanCoder.h"
#include <iostream>

using namespace std;

/*
 * name:      main( )
 * purpose:   check user-given parameters and instantiate and begin HuffmanCoder
 * arguments: number of args and array of args
 * returns:   0
 * effects:   Builds and begins HuffmanCoder
 */
int main(int argc, char *argv[]) {
    // Check if the proper # of args provided

    if (argc != 4 or
        (argv[1] != string("zap") and argv[1] != string("unzap"))) {
        cerr << "Usage: ./zap [zap | unzap] inputFile outputFile\n";
        exit(EXIT_FAILURE);
    }
    HuffmanCoder huffCoder = HuffmanCoder();
    // encode or decode based off input
    (argv[1] == string("zap")) ? huffCoder.encoder(argv[2], argv[3])
                               : huffCoder.decoder(argv[2], argv[3]);
    return 0;
}

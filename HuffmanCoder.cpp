/*
 *  HuffmanCoder.cpp
 *  Harry Lynch
 *  11/7/2024
 *
 *  Implementation of the main driver class for both the Huffman encoding and
 *  decoding processes. This includes all the proceses for creating a huffman
 *  tree given a set of inputs from counting frequencies to building the binary
 *  file on the encoding side. On the decoding side, this file covers the whole
 *  process from deserialization to re-conversion
 *
 */

#include "HuffmanCoder.h"
using namespace std;

/*
 * name:      HuffmanCoder::HuffmanCoder( )
 * purpose:   Constructor for the HuffmanCoder class, initializing the root
 *            pointer.
 * arguments: none
 * returns:   none
 * effects:   initializes the root of
 * the Huffman tree to nullptr.
 */
HuffmanCoder::HuffmanCoder() { this->root = nullptr; }

/*
 * name:      HuffmanCoder::~HuffmanCoder( )
 * purpose:   Destructor for the HuffmanCoder class, cleaning up resources.
 * arguments: none
 * returns:   none
 * effects:   closes input and output streams and frees memory for the Huffman
 * tree.
 */
HuffmanCoder::~HuffmanCoder() {
    this->inputStream.close();
    this->outputStream.close();
    if (this->root)
        cleanTree(this->root);
}

/*
 * name:      HuffmanCoder::encoder( )
 * purpose:   Compresses the input file using Huffman coding and writes the
 *            output.
 * arguments: inputFile - name of the input file to be compressed
 *            outputFile - name of the output file to save the encoded data
 * returns:   none
 * effects:   encodes the input file and saves the result to the output file.
 */
void HuffmanCoder::encoder(const string &inputFile, const string &outputFile) {
    open_or_die(this->inputStream, inputFile);
    open_or_die(this->outputStream, outputFile);

    this->count_freqs(inputStream);
    if (this->freqMap.empty()) {
        cout << inputFile << " is empty and cannot be compressed." << endl;
        // writeZapFile(outputFile, nullptr, "");
        return;
    }
    this->buildTree();
    this->createCharCodes();
    string binString = this->asciiToBinary(inputFile);
    string serializedTree = this->serialize_tree(this->root);
    writeZapFile(outputFile, serializedTree, binString);

    cout << "Success! Encoded given text using " << binString.length()
         << " bits." << endl;
    return;
}

/*
 * name:      HuffmanCoder::decoder( )
 * purpose:   Decompresses the encoded file and writes the decoded output.
 * arguments: inputFile - name of the input file to be decompressed
 *            outputFile - name of the output file to save the decoded data
 * returns:   none
 * effects:   decodes the input file and saves the result to the output file.
 */
void HuffmanCoder::decoder(const string &inputFile, const string &outputFile) {
    open_or_die(this->inputStream, inputFile);
    open_or_die(this->outputStream, outputFile);
    pair<string, string> encodedStrs = readZapFile(inputFile);
    deserialize_tree(encodedStrs.first);
    istringstream ss;
    string ASCII;
    if (this->root->is_leaf()) {
        ASCII = string(int(encodedStrs.second.length()), root->get_val());
    } else
        ASCII = binaryToASCII(encodedStrs.second);
    ss.str(ASCII);
    this->outputStream << ss.str();
    return;
}

/*
 * name:      count_freqs( )
 * purpose:   Given a string containing ASCII characters, read in each character
 *            and their frequencies into a hashmap.
 * arguments: input stream to take characters from
 * returns:   none
 * effects:   creates and fills a map with char freqs.
 */
void HuffmanCoder::count_freqs(istream &text) {
    char holder;
    // indicates to the input stream to accept whitespace characters.
    text >> noskipws;
    while (text >> holder) {
        this->freqMap[holder]++;
    }
}

/*
 * name:      HuffmanCoder::buildTree( )
 * purpose:   Builds the Huffman tree based on character frequencies.
 * arguments: none
 * returns:   none
 * effects:   constructs the Huffman tree and assigns it to the root.
 */
void HuffmanCoder::buildTree() {
    // iterate over the map and print the key/value in the format requested
    priority_queue<HuffmanTreeNode *, vector<HuffmanTreeNode *>, NodeComparator>
        tree_pq;
    auto it = this->freqMap.begin();
    for (; it != this->freqMap.end(); ++it) {
        tree_pq.push(new HuffmanTreeNode(it->first, it->second));
    }
    while (not tree_pq.empty()) {
        if (tree_pq.size() == 1) {
            this->root = tree_pq.top();
            tree_pq.pop();
            continue;
        }
        HuffmanTreeNode *top1 = tree_pq.top();
        tree_pq.pop();
        HuffmanTreeNode *top2 = tree_pq.top();
        tree_pq.pop();
        int parentFreq = top1->get_freq() + top2->get_freq();
        HuffmanTreeNode *parent =
            new HuffmanTreeNode('\0', parentFreq, top1, top2);
        tree_pq.push(parent);
    }
}

/*
 * name:      HuffmanCoder::createCharCodes( )
 * purpose:   Generates binary Huffman codes for each character in the tree.
 * arguments: none
 * returns:   none
 * effects:   fills the codeMap with the binary representation of each
 *            character.
 */
void HuffmanCoder::createCharCodes() {
    string charCode = "";
    if (this->root->get_val() != '\0') {
        codeMap[root->get_val()] = '0';
        return;
    }
    charCodeHelper(this->root->get_left(), charCode, '0');
    charCodeHelper(this->root->get_right(), charCode, '1');
}

/*
 * name:      HuffmanCoder::charCodeHelper( )
 * purpose:   Recursively generates the Huffman codes for the tree nodes.
 * arguments: curr - current tree node
 *            currCode - current binary code being built
 *            leftOrRight - indicates whether the direction is left or right
 * returns:   none
 * effects:   updates codeMap with Huffman codes for leaf nodes.
 */
void HuffmanCoder::charCodeHelper(HuffmanTreeNode *curr, string &currCode,
                                  char leftOrRight) {
    if (not curr)
        return;
    currCode.push_back(leftOrRight);
    if (curr->is_leaf()) {
        codeMap[curr->get_val()] = currCode;
        currCode.pop_back();
        return;
    }
    charCodeHelper(curr->get_left(), currCode, '0');
    charCodeHelper(curr->get_right(), currCode, '1');
    currCode.pop_back();
}

/*
 * name:      HuffmanCoder::asciiToBinary( )
 * purpose:   Converts an ASCII input file into a binary string using Huffman
 *            codes.
 * arguments: inputFile - name of the file to be converted returns:
 *            binary string representing the ASCII data
 * effects:   reads the file and generates a binary string of Huffman codes.
 */
string HuffmanCoder::asciiToBinary(const string &inputFile) {
    this->inputStream.close();
    open_or_die(inputStream, inputFile);
    inputStream >> noskipws;
    char holder;
    string binString = "";
    while (inputStream >> holder) {
        binString += this->codeMap[holder];
    }
    return binString;
}

/*
 * name:      serialize_tree( )
 * purpose:   Given the root of a huffTree, create a serialized string where 'I'
 *            denotes an internal node and 'L' followed by a char is a leaf
 *            which represents a character to be encoded.
 * arguments: root of a huffman tree
 * returns:   serialized string
 * effects:   creates a serialized string, does not mutate tree.
 */
string HuffmanCoder::serialize_tree(HuffmanTreeNode *curr) {
    if (curr == nullptr)
        return "";
    // If leaf, add the L and the char it represents to the string
    else if (curr->is_leaf())
        return "L" + string(1, curr->get_val());

    HuffmanTreeNode *lt = curr->get_left();
    HuffmanTreeNode *rt = curr->get_right();
    // if internal, traverse left and right and add them to the string
    return "I" + serialize_tree(lt) + serialize_tree(rt);
}

/*
 * name:      deserialize_tree( )
 * purpose:   given a serialized string, translate back into a huffman tree
 * arguments: serialized tree
 * returns:   node pointing to the root of the deserialized tree
 * effects:   creates a huffman tree
 */
void HuffmanCoder::deserialize_tree(const string &s) {
    // Declare an incrementing integer for the helper
    int index = 0;
    // Set the root to the first call of the helper
    this->root = deserialize_helper(s, index);
}

/*
 * name:      deserialize_helper( )
 * purpose:   a recursive aid to the deserialization process that traverses tree
 * arguments: serialized string and the current index to read from
 * returns:   a deserialized node of a huffman tree
 * effects:   traverses and creates a huffman tree
 */
HuffmanTreeNode *HuffmanCoder::deserialize_helper(const string &s, int &index) {
    if (index >= int(s.length()))
        return nullptr;
    // If internal node, create a node holding null and recurse left and then rt
    if (s[index] == 'I') {
        HuffmanTreeNode *newNode = new HuffmanTreeNode('\0');
        newNode->set_left(deserialize_helper(s, ++index));
        newNode->set_right(deserialize_helper(s, ++index));
        return newNode;
    }
    // If leaf, set leaf value to next char in the string then ret node.
    else if (s[index] == 'L') {
        // Get to character to input into node
        ++index;
        HuffmanTreeNode *newNode = new HuffmanTreeNode(s[index]);
        return newNode;
    }
    // shouldn't come to this but if something goes wrong return null.
    return nullptr;
}

/*
 * name:      HuffmanCoder::binaryToASCII( )
 * purpose:   Converts a binary string back into an ASCII string using the
 *            Huffman tree.
 * arguments: binString - binary string to be converted
 * returns:   ASCII string decoded from the binary input
 * effects:   decodes the binary string into a readable ASCII string.
 */
string HuffmanCoder::binaryToASCII(string &binString) {
    string res = "";
    int index = 0;
    int binLen = int(binString.length());
    HuffmanTreeNode *curr = this->root;
    while (index <= binLen) {
        if (curr == nullptr) {
            index = binLen + 1;
            continue;
        }

        if (curr->is_leaf()) {
            res += string(1, curr->get_val());
            if (index == binLen) {
                return res;
            }
            curr = this->root;
            continue;
        }

        if (index == binLen) {
            throw runtime_error("Encoding did not match Huffman tree.");
        }
        // Traverse left or right
        if (binString[index] == '0')
            curr = curr->get_left();
        else
            curr = curr->get_right();

        ++index;
    }
    return res;
}

/*
 * name:      cleanTree( )
 * purpose:   recycles all memory within a huffman tree
 * arguments: root of the tree to delete
 * returns:   none
 * effects:   post-order delete of a huffman tree
 */
void HuffmanCoder::cleanTree(HuffmanTreeNode *huffRoot) {
    if (not huffRoot)
        return;
    cleanTree(huffRoot->get_left());
    cleanTree(huffRoot->get_right());
    delete huffRoot;
}

/*
 * name:      HuffmanCoder::debug_printMaps( )
 * purpose:   Prints the frequency map and/or the code map for debugging.
 * arguments: map - the type of map to print ("freq", "code", or "both")
 * returns:   none
 * effects:   prints either the frequency map, code map, or both.
 */
void HuffmanCoder::debug_printMaps(std::string map) {
    if (map == "freq" or map == "both") {
        for (auto freq : this->freqMap) {
            cout << freq.first << ": " << freq.second << endl;
        }
    }
    if (map == "code" or map == "both") {
        cout << "PRINTING CODES: " << endl;
        for (auto code : this->codeMap) {
            cout << code.first << ": " << code.second << endl;
        }
    }
}

/*
 * name:      HuffmanCoder::open_or_die( )
 * purpose:   Opens a file and throws an error if it cannot be opened.
 * arguments: stream - the stream to open (input or output)
 *            fileName - the file name to open
 * returns:   none
 * effects:   throws an exception if the file cannot be opened.
 */
template <typename streamtype>
void HuffmanCoder::open_or_die(streamtype &stream, string fileName) {
    stream.open(fileName);
    if (not stream.is_open()) {
        stringstream ss;
        ss << "Unable to open file " << fileName;
        throw runtime_error(ss.str());
    }
}
/*
Collection of utilities that are beneficial while testing / debugging the
various steps of zapping / unzapping.

This module provides functions to:
- Make a tree based on the tree in figure 1
- Print a tree in a hierarchical fashion
- Determine if two trees are equal

Please read the function headers below for more information.

Author: Chami Lamelas (slamel01) and Milod Kazerounian
Date: Jan, April 2023
*/

#ifndef _ZAP_UTIL_H
#define _ZAP_UTIL_H

#include "HuffmanTreeNode.h"

#include <string>
#include <utility>

using namespace std;

/*
Constructs a tree that looks like the tree in figure 1 of the spec.

Frequencies are set in the nodes of the tree that are possible for the
tree to look as it does in the spec as if it were built using the
build Huffman tree algorithm outlined in the spec.

In particular, we suppose characters a, b, e, and f have frequency 1
and characters c and d have frequency 2.

Hence the tree constructed by this algorithm, if printed, would look
something like this:

                            []|8

            []|4                            []|4

    []|2            []|2            c |2            d |2

a |1    b |1    e |1    f |1

Parameters:
    internalNodeVal - The character that is used as the value for
                      internal nodes. You have to choose this when
                      writing your build Huffman tree function.  '\0'
                      is a good choice for example.

Returns:
    The root to a tree created as described above. Note, the root is a
    pointer to memory on the heap, so you will need to free this if
    you're testing your code with valgrind.
*/
HuffmanTreeNode *makeFigure1Tree(char internalNodeVal);

/*
Prints a tree in a nice manner, level by level with structure and both
node values and frequency.

Given that the internal node value can be set to non-printable
characters, their value is replaced by the string []. So, an internal
node with frequency 10 would appear in the printed output as []|10.

Output is printed to stdout.

WARNING: It is recommended this function is only used on smaller trees
as eventually, with deeper trees, the width of text that is printed to
stdout will quickly increase and will most likely wrap around in your
terminal.

Parameters:
    root - Pointer to the root of the tree to print.
    internalNodeVal - The value used in internal nodes of the tree. You have to
                      choose this when writing your build Huffman tree
                      function. '\0' is a good choice for example.
Returns:
    Nothing (void).
*/
void printTree(HuffmanTreeNode *root, char internalNodeVal);

/*
Determines whether the trees rooted at two nodes are equal.

Tree equality is defined recursively as follows.
    1. If both trees are empty, they're equal. If one is empty and the
       other isn't, they are not.
    2. If both trees are not empty, the roots of the two trees must
       have equal value (depending on whether they are leaves and
       checkInternalValues) and frequency (depending on
       checkFrequency) as well as having equal left and right
       subtrees.

Parameters:
    root1 - Pointer to the root of a tree.
    root2 - Pointer to the root of another tree.
    checkFrequency - If set to true, the frequency must also be equal
                     in step 2.
    checkInternalValues - If set to true, the values of internal nodes
                          (in addition to leaves) must be equal
                          between the 2 trees.

Returns:
    true if the two trees are equal as defined above and false otherwise.

Example Usage:
    The autograder tests for deserialize_tree in phase 1 use this
    function with checkFrequency and checkInternalValues set to false.
*/
bool treeEquals(HuffmanTreeNode *root1, HuffmanTreeNode *root2,
                bool checkFrequency, bool checkInternalValues);

/* Writes to a binary file named ++filename++.
 * ++serial_tree++ should be a serialized Huffman tree, and
 * ++bitstring++ should be a string of 0s and 1s
 * comprising the encoded text.
 *
 * Possible exception: Will throw a runtime_error if given
 * ++bitstring++ is larger than 2^32 bits.
 */
void writeZapFile(const string &filename, const string &serial_tree,
                  const string &bitstring);

/* Reads from zapped file named ++filename++.
 * returns a pair. A pair is a special C++
 * data structure that holds 2 elements. The
 * first element contains the serialized Huffman tree, and
 * the second element contains the encoded text.
 */
pair<string, string> readZapFile(const string &filename);

#endif

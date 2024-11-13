/*
Defines HuffmanTreeNode class.
*/

#ifndef __HUFFMANTREENODE_H__
#define __HUFFMANTREENODE_H__

class HuffmanTreeNode {
  public:
    /*
     * Constructor #1: sets val to c, freq to f, left to l, and right to r.
     */
    HuffmanTreeNode(char c, int f, HuffmanTreeNode *l, HuffmanTreeNode *r);

    /* The following constructors are custom made for individual portions of
     * the assignment where they might be useful. Feel free to use them where
     * appropriate, or use the standard constructor if you prefer. */

    /*
     * Constructor #2: sets val to c, freq to f, and
     * sets left and right to nullptr by default.
     */
    HuffmanTreeNode(char c, int f);

    /*
     * Constructor #3: sets left to l, right to r,
     * sets freq to DEFAULT_FREQUENCY, sets val to
     * DEFAULT_VALUE
     */
    HuffmanTreeNode(HuffmanTreeNode *l, HuffmanTreeNode *r);

    /*
     * Constructor #4: sets val to DEFAULT_VALUE, freq to f, and
     * sets left to l, and right to r
     */

    HuffmanTreeNode(int f, HuffmanTreeNode *l, HuffmanTreeNode *r);

    /*
     * Constructor #5: sets val to c, freq to DEFAULT_FREQUENCY,
     * sets left to nullptr, and right to nullptr
     */

    HuffmanTreeNode(char c);

    /*
     * Helper function for determining if this node is a leaf.
     */
    bool is_leaf() const;

    /*
     * getters for the private member variables
     */
    char get_val() const;
    int get_freq() const;
    HuffmanTreeNode *get_left() const;
    HuffmanTreeNode *get_right() const;

    /*
     * setters for left and right
     */
    void set_left(HuffmanTreeNode *n);
    void set_right(HuffmanTreeNode *n);

  private:
    static const int DEFAULT_FREQUENCY = -1;
    static const char DEFAULT_VALUE = '\0';
    /*
     * The character stored within this node.
     * Should be '\0' for internal nodes.
     */
    char val;

    /*
     * The frequency of this character,
     * or the frequency for all descendant characters for internal nodes.
     */
    int freq;

    /*
     * Left and right children.
     */
    HuffmanTreeNode *left;
    HuffmanTreeNode *right;
};

/* A comparator class: defines a comparison function so that nodes can
 * be used in a minheap.
 * The comparison function compares two nodes based on their frequencies.
 * Returns n1->get_freq() > n2->get_freq().
 */
class NodeComparator {
  public:
    bool operator()(const HuffmanTreeNode *n1, const HuffmanTreeNode *n2);
};

#endif

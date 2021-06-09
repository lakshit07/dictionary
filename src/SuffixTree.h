#pragma once

#include <src/SuffixTreeUtil.h>

#include <string>
#include <vector>

template <typename EleT>
class SuffixTree {
    // Data type for a collection of EleT
    using collectionT = std::vector<EleT>;
    // Map containing the index to element mapping
    std::unordered_map<int32_t, collectionT> m_stringMap;
    // Index of the element added
    int32_t m_index;
    // Root of the tree
    Node<EleT> m_root;
    // Sink node
    Sink<EleT> m_sink;
    // Maximum length of the word added
    static constexpr int32_t MAX_LEN = 500;
    // Start and end delimiters
    static constexpr char START = '$';
    static constexpr char END = '#';

public:
    SuffixTree() {
        m_root.m_suffixLink = &m_sink;
        m_sink.m_suffixLink = &m_root;
        m_index = 0;
    }

    virtual ~SuffixTree();

private:

    /**
     * Perform the insertion of the node in the suffix tree
     * @param it the string to be inserted
     * @param index id of the string
     */
    void insertUtil(const collectionT & word, int32_t index);

    /**
     * Find the location to insert the new word
     * @param word word to be inserted
     * @param store current state of active store
     * @return index at which the new node needs to be created wrt the active store
     */
    std::optional<int32_t> findPivot(const collectionT& word, ActiveStore<EleT> *store);

    /**
     * The main part of the Ukkonen algorithm which walks along the active store node
     * and creates nodes on the way
     * @param pNode current active store node
     * @param edgeString current length whose suffix creation is left
     * @return New active store after node creation/insertion
     */
    ActiveStore<EleT> update(Node<EleT> *pNode, EdgeString edgeString);

    /**
     * Returns the canonical pair of (pNode, edgeString)
     * @param pNode the current node
     * @param edgeString edge string
     * @return canonical pair
     */
    ActiveStore<EleT> canonize(Node<EleT> *pNode, EdgeString edgeString);

    /**
     * Checks if (n, eStr) is an end point in the suffix tree.
     * If not, create a new node
     * @param n Current node
     * @param eStr edge to be inserted
     * @param ch last character of string added
     * @param str string to be inserted
     * @param nNode new node created
     * @return if split happened or not
     */
    bool testAndSplit(Node<EleT>* n, EdgeString eStr, EleT ch, const collectionT& str,
                      Node<EleT>** nNode);

    void populateIndicesUtil(Node<EleT>& node);

public:
    /**
     * Insert the word in the suffix tree
     * @param word word to be inserted
     */
    void insert(const std::string& word);

    /**
     * Computes the set of indices of words which have the given substring
     * @param word substring to be looked for
     * @return set of indices
     */
    std::optional<std::unordered_set<int32_t>> substringSet(const collectionT& word);

    // Suffix, Prefix and Substring lookup and counts
    std::vector<std::string> searchPrefix(const std::string& prefix) ;
    std::vector<std::string> searchSuffix(const std::string& suffix) ;
    std::vector<std::string> searchSubstring(const std::string& substring) ;

    uint32_t countPrefix(const std::string& prefix) ;
    uint32_t countSuffix(const std::string& suffix) ;
    uint32_t countSubstring(const std::string& substring) ;

    /**
     * Populates the internal nodes with the indices of words whose suffixes
     * are in its subtree
     */
    void populateIndices();
};

#include "SuffixTree.hpp"
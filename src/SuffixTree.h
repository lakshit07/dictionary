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
    static constexpr char START = '$';
    static constexpr char END = '#';

public:
    SuffixTree() {
        m_root.m_suffixLink = &m_sink;
        m_sink.m_suffixLink = &m_root;
        m_index = 0;
    }

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
     * TODO: add documentation
     * @param pNode
     * @param edgeString
     * @return
     */
    ActiveStore<EleT> update(Node<EleT> *pNode, EdgeString edgeString);

    /**
     * TODO: add documentation
     * @param pNode
     * @param edgeString
     * @return
     */
    ActiveStore<EleT> canonize(Node<EleT> *pNode, EdgeString edgeString);

    /**
     * TODO : add documentation
     * @param n
     * @param eStr
     * @param ch
     * @param str
     * @param nNode
     * @return
     */
    bool testAndSplit(Node<EleT>* n, EdgeString eStr, EleT ch, const collectionT& str,
                      Node<EleT>** nNode);

    void printNode(const Node<EleT>* node, bool sameLine, int32_t padding, EdgeString eStr) const;

    void populateIndicesUtil(Node<EleT>& node);

public:
    void printTree() const ;
    void insert(const std::string& word);

    std::optional<std::unordered_set<int32_t>> substringSet(const collectionT& word);

    std::vector<std::string> searchPrefix(const std::string& prefix) ;
    std::vector<std::string> searchSuffix(const std::string& suffix) ;
    std::vector<std::string> searchSubstring(const std::string& substring) ;

    uint32_t countPrefix(const std::string& prefix) ;
    uint32_t countSuffix(const std::string& suffix) ;
    uint32_t countSubstring(const std::string& substring) ;

    void populateIndices();
};

#include "SuffixTree.hpp"
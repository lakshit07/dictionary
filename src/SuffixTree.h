#pragma once

#include <src/SuffixTreeUtil.h>

#include <string>
#include <vector>

class SuffixTree {
    // Set containing all words in the tree
    std::unordered_set<std::string> m_words;
    // Map containing the index,string pairs
    std::unordered_map<int32_t, std::string> m_stringMap;
    // Index of the word added
    int32_t m_index = 0;
    // Root of the tree
    Node m_root;
    // Maximum length of the word added
    int32_t MAX_LEN = 500;

    /**
     * Perform the insertion of the node in the suffix tree
     * @param it iterator to the set containing the string
     * @param index id of the string
     */
    void insertUtil(std::unordered_set<std::string>::const_iterator it,
                    int32_t index);

    /**
     * Find the location to insert the new word
     * @param word
     * @param store
     * @return
     */
    std::optional<int32_t> findPivot(const std::string &word, ActiveStore *store);

    /**
     * TODO: add documentation
     * @param pNode
     * @param edgeString
     * @return
     */
    ActiveStore update(Node *pNode, EdgeString edgeString);

    /**
     * TODO: add documentation
     * @param pNode
     * @param edgeString
     * @return
     */
    ActiveStore canonize(Node *pNode, EdgeString edgeString);

    /**
     * TODO : add documentation
     * @param n
     * @param eStr
     * @param ch
     * @param str
     * @param nNode
     * @return
     */
    bool testAndSplit(Node* n, EdgeString eStr, char ch, const std::string& str,
                      Node** nNode);

public:
    void insert(const std::string& word);

    std::vector<std::string> searchPrefix(const std::string& prefix) const;
    std::vector<std::string> searchSuffix(const std::string& suffix) const;
    std::vector<std::string> searchSubstring(const std::string& substring) const;

    uint32_t countPrefix(const std::string& prefix) const;
    uint32_t countSuffix(const std::string& suffix) const;
    uint32_t countSubstring(const std::string& substring) const;

};


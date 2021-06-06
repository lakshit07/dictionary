#pragma once

#include <src/SuffixTreeUtil.h>

#include <string>
#include <vector>

class SuffixTree {
    // Set containing all words in the tree
    const std::unordered_set<std::string> m_words;
    // Index of the word added
    int32_t m_index = 0;
    // Root of the tree
    Node m_root;

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
    static std::optional<int32_t> findPivot(const std::string &word, ActiveStore *store);

public:
    void insert(const std::string& word);

    std::vector<std::string> searchPrefix(const std::string& prefix) const;
    std::vector<std::string> searchSuffix(const std::string& suffix) const;
    std::vector<std::string> searchSubstring(const std::string& substring) const;

    uint32_t countPrefix(const std::string& prefix) const;
    uint32_t countSuffix(const std::string& suffix) const;
    uint32_t countSubstring(const std::string& substring) const;
};


#pragma once

#include <string>
#include <vector>

class SuffixTree {
public:
    void insert(const std::string& word);

    std::vector<std::string> searchPrefix(const std::string& prefix) const;
    std::vector<std::string> searchSuffix(const std::string& suffix) const;
    std::vector<std::string> searchSubstring(const std::string& substring) const;

    uint32_t countPrefix(const std::string& prefix) const;
    uint32_t countSuffix(const std::string& suffix) const;
    uint32_t countSubstring(const std::string& substring) const;
};


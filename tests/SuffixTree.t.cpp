#include <src/Dictionary.h>

#include <gtest/gtest.h>

class SuffixTreeTest : public ::testing::Test {
public:
    SuffixTree<int32_t> m_tree;
    std::vector<std::string> words = {
            "aardwolf",    "aardwolves",
            "aarhus",      "aaron",
            "ab",          "aba",
            "abandoning",  "abashing",
            "abasing",     "abdicating",
            "abracadabra", "academically",
            "ambuscading", "arcadian",
            "avocados",    "avocado",
            "barricado",   "barricoes",
            "bassanio" ,   "banana",
    };

    void SetUp() override {
        for (const auto& word : words) {
            m_tree.insert(word);
        }
        m_tree.populateIndices();
    }
};

TEST_F(SuffixTreeTest, PrefixTest) {
    std::string prefix = "ab";
    const auto& words = m_tree.searchPrefix(prefix);

    ASSERT_EQ(words.size(), 7);
    ASSERT_EQ(m_tree.countPrefix(prefix), 7);

    for (const std::string& str : words) {
        ASSERT_GE(str.size(), prefix.size());
        for (int i = 0 ; i < prefix.size() ; ++i) {
            ASSERT_EQ(str[i], prefix[i]);
        }
    }
}

TEST_F(SuffixTreeTest, SuffixTest) {
    std::string suffix = "ing";
    const auto& words = m_tree.searchSuffix(suffix);

    ASSERT_EQ(words.size(), 5);
    ASSERT_EQ(m_tree.countSuffix(suffix), 5);

    for (const std::string& str : words) {
        ASSERT_GE(str.size(), suffix.size());
        for (int i = 0 ; i < suffix.size() ; ++i) {
            ASSERT_EQ(str[str.size() - suffix.size() + i], suffix[i]);
        }
    }
}

TEST_F(SuffixTreeTest, SubstringTest) {
    std::string substring = "cad";
    const auto& words = m_tree.searchSubstring(substring);

    ASSERT_EQ(words.size(), 7);
    ASSERT_EQ(m_tree.countSubstring(substring), 7);

    for (const auto& word : words) {
        ASSERT_TRUE(word.find(substring) != std::string::npos);
    }
}
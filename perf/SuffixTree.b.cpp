#include <src/SuffixTree.h>

#include <benchmark/benchmark.h>
#include <random>

class SuffixTreePerf : public benchmark::Fixture {
public:
    SuffixTree<int32_t> m_tree;
};

BENCHMARK_F(SuffixTreePerf, TenThousandInsertions) (benchmark::State& st) {
    for (auto _ : st) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 25);

        int wordLen = 100;
        int numInsertions = 10000;
        std::string word;

        for (int i = 0 ; i < numInsertions ; ++i) {
            for (int j = 0 ; j < wordLen ; ++j) {
                word += static_cast<char>('a' + distrib(gen));
            }
            m_tree.insert(word);
            word.clear();
        }
    }
}

BENCHMARK_F(SuffixTreePerf, Queries) (benchmark::State& st) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 25);

    int wordLen = 95;
    int numInsertions = 10000;
    std::string word;

    for (int i = 0 ; i < numInsertions ; ++i) {
        word += 'a';       // start with a
        for (int j = 0 ; j < wordLen ; ++j) {
            word += static_cast<char>('a' + distrib(gen));
        }
        word += 'a';       // ends with a
        m_tree.insert(word);
        word.clear();
    }

    m_tree.populateIndices();

    for (auto _ : st) {
        const auto& prefixes = m_tree.searchPrefix("a");
        const auto& suffixes = m_tree.searchSuffix("a");
        const auto& substrings = m_tree.searchSubstring("a");
    }
}


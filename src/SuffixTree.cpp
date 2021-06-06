#include "SuffixTree.h"

void SuffixTree::insert(const std::string &word) {
    // If the word already exist (duplicate) don't insert
    const auto it = m_words.find(word);
    if (it == m_words.cend())
        return;

    m_index++;
    insertUtil(it, m_index);
}

void SuffixTree::insertUtil(std::unordered_set<std::string>::const_iterator it,
                            int32_t index) {
    ActiveStore active{&m_root, index, 0};

    auto pivot = findPivot(*it, &active);
    if (!pivot) {
        return;
    }

    int offset = pivot.value();
    for (int i = offset ; i < (*it).size() ; ++i) {

    }

}

std::optional<int32_t> SuffixTree::findPivot(const std::string &word, ActiveStore *store) {
    if (store->m_length > word.size()) {
        return std::nullopt;
    }

    int indx = store->m_length;
    bool matching = true;

    while (matching) {
        Node* currNode = store->m_node;
        auto& edge = currNode->m_transitionMap[word[indx]];

        if (edge.m_next != nullptr) {
            int32_t offset;
            const std::string& edgeStr = *edge.m_string;
            for (offset = 1 ; offset <= edge.m_right - edge.m_left ; ++offset) {
                if (indx + offset >= word.size()) {
                    matching = false;
                    break;
                }
                if (word[indx + offset] != edgeStr[edge.m_left + offset]) {
                    return indx + offset;
                }
            }

            if (matching) {
                store->m_node = edge.m_next;
                indx += offset;
                store->m_length = indx;
            }
        }
        else {
            return indx;
        }
    }

    return std::nullopt;
}


std::vector<std::string> SuffixTree::searchPrefix(const std::string &prefix)const {
    return std::vector<std::string>();
}

std::vector<std::string> SuffixTree::searchSuffix(const std::string &suffix) const {
    return std::vector<std::string>();
}

std::vector<std::string> SuffixTree::searchSubstring(const std::string &substring) const {
    return std::vector<std::string>();
}

uint32_t SuffixTree::countPrefix(const std::string &prefix) const {
    return 0;
}

uint32_t SuffixTree::countSuffix(const std::string &suffix) const {
    return 0;
}

uint32_t SuffixTree::countSubstring(const std::string &substring) const {
    return 0;
}





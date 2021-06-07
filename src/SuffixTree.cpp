#include "SuffixTree.h"

void SuffixTree::insert(const std::string &word) {
    // If the word already exist (duplicate) don't insert
    const auto it = m_words.find(word);
    if (it == m_words.cend())
        return;

    m_index++;
    m_stringMap[m_index] = word;
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
        EdgeString eStr {index, active.m_length, i};
        active = update(active.m_node, eStr);
        eStr.m_left = active.m_length;
        active = canonize(active.m_node, eStr);
    }
}

bool SuffixTree::testAndSplit(Node* n, EdgeString eStr, char ch, const std::string& str, Node** nNode) {
    char x = str[eStr.m_left];
    int32_t len = eStr.m_right - eStr.m_left;

    if (len >= 0) {
        auto transition = n->next(x);
        auto edge = transition.m_edgeStr;

        const std::string& edgeStr = m_stringMap[edge.m_stringId];
        if (edgeStr[edge.m_left + len + 1] == ch) {
            nNode = &n;
            return true;
        }

        *nNode = new Node();
        Transition newTransition = transition;
        newTransition.m_edgeStr.m_left += len + 1;
        (*nNode)->m_transitionMap[edgeStr[newTransition.m_edgeStr.m_left]] = newTransition;

        transition.m_edgeStr.m_right = transition.m_edgeStr.m_left + len;
        transition.m_next = *nNode;
        n->m_transitionMap[x] = transition;

        return false;
    }
    else {
        Transition transition = n->next(ch);
        *nNode = n;
        return transition.m_next != nullptr;
    }
}

ActiveStore SuffixTree::update(Node *pNode, EdgeString edgeString) {
    Node* old = &m_root;
    Node* r = nullptr;

    EdgeString eStr = edgeString;
    eStr.m_right--;

    const std::string& str = m_stringMap[edgeString.m_stringId];
    ActiveStore active{pNode, edgeString.m_stringId, edgeString.m_left};

    bool reachedEnd = testAndSplit(pNode, eStr, str[eStr.m_right], str, &r);
    while (!reachedEnd) {
        Leaf* leaf = new Leaf();
        r->m_transitionMap[str[eStr.m_right]] = {{edgeString.m_stringId, edgeString.m_right, MAX_LEN}, leaf};

        if (&m_root != old) {
            old->m_suffixLink = r;
        }
        old = r;

        active = canonize(active.m_node->m_suffixLink, eStr);
        edgeString.m_left = active.m_length;
        eStr.m_left = active.m_length;

        reachedEnd = testAndSplit(active.m_node, eStr, str[edgeString.m_right], str, &r);
    }

    if (old != &m_root) {
        old->m_suffixLink = active.m_node;
    }

    return active;
}

ActiveStore SuffixTree::canonize(Node *pNode, EdgeString edgeString) {
    if (edgeString.m_left > edgeString.m_right) {
        return {pNode, edgeString.m_stringId, edgeString.m_left};
    }

    const std::string& str = m_stringMap[edgeString.m_stringId];
    Transition transition = pNode->next(str[edgeString.m_left]);

    int32_t len = transition.m_edgeStr.m_right - transition.m_edgeStr.m_left;
    while (len <= edgeString.m_right - edgeString.m_left) {
        edgeString.m_left += (1 + len);
        pNode = transition.m_next;
        if (edgeString.m_left <= edgeString.m_right) {
            transition = pNode->next(str[edgeString.m_left]);
        }
        len = transition.m_edgeStr.m_right - transition.m_edgeStr.m_left;
    }
    return {pNode, edgeString.m_stringId, edgeString.m_left};
}

std::optional<int32_t> SuffixTree::findPivot(const std::string &word, ActiveStore *store) {
    if (store->m_length > word.size()) {
        return std::nullopt;
    }

    int indx = store->m_length;
    bool matching = true;

    while (matching) {
        Node* currNode = store->m_node;
        auto& transition = currNode->m_transitionMap[word[indx]];

        if (transition.m_next != nullptr) {
            int32_t offset;
            const std::string& edgeStr = m_stringMap[transition.m_edgeStr.m_stringId];

            for (offset = 1;
                 offset <= transition.m_edgeStr.m_right - transition.m_edgeStr.m_left;
                 ++offset)
            {
                if (indx + offset >= word.size()) {
                    matching = false;
                    break;
                }
                if (word[indx + offset] != edgeStr[transition.m_edgeStr.m_left + offset]) {
                    return indx + offset;
                }
            }

            if (matching) {
                store->m_node = transition.m_next;
                indx += offset;
                store->m_length = indx;
            }
        }
        else {
            return indx;
        }
    }

    store->m_length = MAX_LEN;
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







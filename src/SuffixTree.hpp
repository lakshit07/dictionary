#include <iostream>
#include "SuffixTree.h"


template <typename EleT>
void SuffixTree<EleT>::insert(const std::string &word) {

    collectionT element;
    element.push_back(static_cast<EleT>(START));
    for (char ch : word) {
        element.push_back(static_cast<EleT>(ch));
    }
    element.push_back(static_cast<EleT>(END));

    m_index++;
    element.push_back(static_cast<EleT>(m_index));

    m_stringMap[m_index] = element;
    insertUtil(element, m_index);
}

template <typename EleT>
void SuffixTree<EleT>::insertUtil(const collectionT& word, int32_t index) {
    ActiveStore<EleT> active{&m_root, index, 0};

    auto pivot = findPivot(word, &active);
    if (!pivot) {
        return;
    }

    int offset = pivot.value();
    for (int i = offset ; i < word.size() ; ++i) {
        EdgeString eStr {index, active.m_length, i};
        active = update(active.m_node, eStr);
        eStr.m_left = active.m_length;
        active = canonize(active.m_node, eStr);
    }
}

template <typename EleT>
bool SuffixTree<EleT>::testAndSplit(Node<EleT>* n, EdgeString eStr, EleT ch, const collectionT& str, Node<EleT>** nNode) {
    EleT x = str[eStr.m_left];
    int32_t len = eStr.m_right - eStr.m_left;

    if (len >= 0) {
        auto transition = n->next(x);
        auto edge = transition.m_edgeStr;

        const auto& edgeStr = m_stringMap[edge.m_stringId];
        if (edgeStr[edge.m_left + len + 1] == ch) {
            nNode = &n;
            return true;
        }

        *nNode = new Node<EleT>();

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

template <typename EleT>
ActiveStore<EleT> SuffixTree<EleT>::update(Node<EleT> *pNode, EdgeString edgeString) {
    Node<EleT>* old = &m_root;
    Node<EleT>* r = nullptr;

    EdgeString eStr = edgeString;
    eStr.m_right--;

    const auto& str = m_stringMap[edgeString.m_stringId];
    ActiveStore<EleT> active{pNode, edgeString.m_stringId, edgeString.m_left};

    bool reachedEnd = testAndSplit(pNode, eStr, str[edgeString.m_right], str, &r);
    while (!reachedEnd) {
        auto* leaf = new Leaf<EleT>();
        leaf->m_stringIds.insert(edgeString.m_stringId);

        r->m_transitionMap[str[edgeString.m_right]] = {{edgeString.m_stringId, edgeString.m_right, MAX_LEN}, leaf};

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

template <typename EleT>
ActiveStore<EleT> SuffixTree<EleT>::canonize(Node<EleT> *pNode, EdgeString edgeString) {
    if (edgeString.m_left > edgeString.m_right) {
        return {pNode, edgeString.m_stringId, edgeString.m_left};
    }

    const auto& str = m_stringMap[edgeString.m_stringId];
    Transition transition = pNode->next(str[edgeString.m_left]);

    int32_t len;
    while ((len = transition.m_edgeStr.m_right - transition.m_edgeStr.m_left)
            <= edgeString.m_right - edgeString.m_left) {

        edgeString.m_left += (1 + len);
        pNode = transition.m_next;

        if (edgeString.m_left <= edgeString.m_right) {
            transition = pNode->next(str[edgeString.m_left]);
        }
    }
    return {pNode, edgeString.m_stringId, edgeString.m_left};
}

template <typename EleT>
std::optional<int32_t> SuffixTree<EleT>::findPivot(const collectionT& word, ActiveStore<EleT> *store) {
    if (store->m_length > word.size()) {
        return std::nullopt;
    }

    int32_t indx = store->m_length;
    int32_t size = word.size();
    bool matching = true;

    while (matching) {
        auto* currNode = store->m_node;
        auto& transition = currNode->m_transitionMap[word[indx]];

        if (transition.m_next != nullptr) {
            int32_t offset;
            const auto& edgeStr = m_stringMap[transition.m_edgeStr.m_stringId];

            for (offset = 1;
                 offset <= transition.m_edgeStr.m_right - transition.m_edgeStr.m_left;
                 ++offset)
            {
                if (indx + offset >= size) {
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

template <typename EleT>
std::vector<std::string> SuffixTree<EleT>::searchPrefix(const std::string &prefix) {
    collectionT query = {static_cast<EleT>(START)};
    for (char ch : prefix) {
        query.push_back(static_cast<EleT>(ch));
    }
    auto set = substringSet(query);
    if (!set) {
        return {};
    }

    std::vector<std::string> ret;
    for (int32_t strId : set.value()) {
        auto word = m_stringMap[strId];
        std::string sWord;
        for (int i = 1 ; i < word.size() - 2 ; i++)
            sWord += static_cast<char>(word[i]);
        ret.push_back(sWord);
    }

    return ret;
}

template <typename EleT>
std::vector<std::string> SuffixTree<EleT>::searchSuffix(const std::string &suffix)  {
    collectionT query;
    for (char ch : suffix) {
        query.push_back(static_cast<EleT>(ch));
    }
    query.push_back(static_cast<int32_t>(END));

    auto set = substringSet(query);
    if (!set) {
        return {};
    }
    std::vector<std::string> ret;
    for (int32_t strId : set.value()) {
        auto word = m_stringMap[strId];
        std::string sWord;
        for (int i = 1 ; i < word.size() - 2 ; i++)
            sWord += static_cast<char>(word[i]);
        ret.push_back(sWord);
    }
    return ret;
}

template <typename EleT>
std::vector<std::string> SuffixTree<EleT>::searchSubstring(const std::string &substring)  {
    collectionT query;
    for (char ch : substring) {
        query.push_back(static_cast<EleT>(ch));
    }

    auto set = substringSet(query);
    if (!set) {
        return {};
    }
    std::vector<std::string> ret;
    for (int32_t strId : set.value()) {
        auto word = m_stringMap[strId];
        std::string sWord;
        for (int i = 1 ; i < word.size() - 2 ; i++)
            sWord += static_cast<char>(word[i]);
        ret.push_back(sWord);
    }
    return ret;
}

template <typename EleT>
uint32_t SuffixTree<EleT>::countPrefix(const std::string &prefix)  {
    collectionT query = {static_cast<EleT>(START)};
    for (char ch : prefix) {
        query.push_back(static_cast<EleT>(ch));
    }
    auto set = substringSet(query);
    if (!set) {
        return 0;
    }
    return set.value().size();
}

template <typename EleT>
uint32_t SuffixTree<EleT>::countSuffix(const std::string &suffix)  {
    collectionT query;
    for (char ch : suffix) {
        query.push_back(static_cast<EleT>(ch));
    }
    query.push_back(static_cast<int32_t>(END));

    auto set = substringSet(query);
    if (!set) {
        return {};
    }
    return set.value().size();
}

template <typename EleT>
uint32_t SuffixTree<EleT>::countSubstring(const std::string &substring)  {
    collectionT query;
    for (char ch : substring) {
        query.push_back(static_cast<EleT>(ch));
    }

    auto set = substringSet(query);
    if (!set) {
        return {};
    }

    return set.value().size();
}

template <typename EleT>
void SuffixTree<EleT>::printNode(const Node<EleT> *node, bool sameLine, int32_t padding,
                           EdgeString eStr) const {
    int32_t delta = 0;
    if (!sameLine) {
        for (int i = 0 ; i < padding ; i++)
            std::cout << " ";
    }

    if (eStr.m_left <= eStr.m_right) {
        auto s = m_stringMap.find(eStr.m_stringId)->second;
        for (int i = eStr.m_left; i <= eStr.m_right and i < s.size(); i++)
            std::cout << s[i] << " ";

        std::cout << " - | ";
        for (int x : node->m_stringIds)
            std::cout << x << " ";
        std::cout << "| ";

        delta = eStr.m_right - eStr.m_left + 2;
        if (eStr.m_right == MAX_LEN)
            delta = s.size() - eStr.m_left;
    }

    sameLine = true;
    for (auto x : node->m_transitionMap) {
        printNode(x.second.m_next, sameLine, padding + delta, x.second.m_edgeStr);
        sameLine = false;
    }

    if (sameLine)
        std::cout << "##" << std::endl;
}

template <typename EleT>
void SuffixTree<EleT>::printTree() const {
    printNode(&m_root, true, 0, {0, 0, -1});
}

template <typename EleT>
std::optional<std::unordered_set<int32_t>> SuffixTree<EleT>::substringSet(const collectionT& word) {
    Node<EleT>* current = &m_root;
    int32_t indx = 0;
    int len = word.size();

    while (true) {
        const auto& transition = current->next(word[indx]);
        if (transition.m_next == nullptr) {
            return std::nullopt;
        }

        const auto& edgeWord = m_stringMap[transition.m_edgeStr.m_stringId];
        for (int i = 1 ; i <= transition.m_edgeStr.m_right - transition.m_edgeStr.m_left ; i++) {
            if (indx + i >= len) {
                return transition.m_next->m_stringIds;
            }
            if (word[indx + i] != edgeWord[transition.m_edgeStr.m_left + i]) {
                return std::nullopt;
            }
        }

        indx += transition.m_edgeStr.m_right - transition.m_edgeStr.m_left + 1;
        current = transition.m_next;
        if (indx == len)
            return current->m_stringIds;
    }
}

template<typename EleT>
void SuffixTree<EleT>::populateIndices() {
    populateIndicesUtil(m_root);
}

template<typename EleT>
void SuffixTree<EleT>::populateIndicesUtil(Node<EleT> &node) {
    for (auto& [ele, trans] : node.m_transitionMap) {
        populateIndicesUtil(*trans.m_next);
        const auto& childIds = trans.m_next->m_stringIds;
        node.m_stringIds.insert(childIds.begin(), childIds.end());
    }
}







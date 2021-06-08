#pragma once

#include <cstdint>
#include <unordered_map>
#include <unordered_set>

// Forward declaration
template <typename EleT>
struct Node;

/**
 * String on the edge of a Transition
 */
struct EdgeString {
    int32_t m_stringId, m_left, m_right;

    EdgeString() : m_stringId(0), m_left(0), m_right(0) {}

    EdgeString(int32_t stringId, int32_t left, int32_t right) :
        m_stringId(stringId), m_left(left), m_right(right) {}
};

template <typename EleT>
struct Transition {
    EdgeString m_edgeStr;
    Node<EleT>* m_next;

    Transition() : m_next(nullptr) {}
    Transition(const EdgeString &mEdgeStr, Node<EleT> *mNext) : m_edgeStr(mEdgeStr),
                                                          m_next(mNext) {}
};

/**
 * Node representation of the suffix tree
 */
template <typename EleT>
struct Node {
    std::unordered_map<EleT, Transition<EleT>> m_transitionMap;
    std::unordered_set<int32_t> m_stringIds;
    Node* m_suffixLink;

    Node() : m_suffixLink(nullptr) {}

    virtual Transition<EleT> next(EleT ch) {
        if (m_transitionMap.find(ch) == m_transitionMap.end()) {
            return {{0, 0, -1}, nullptr};
        }
        return m_transitionMap[ch];
    }
};

template <typename EleT>
struct Sink : Node <EleT> {
    Transition<EleT> next(EleT ch) override {
        return Transition({0, 0, 0}, this->m_suffixLink);
    }
};

template <typename EleT>
struct Leaf : Node <EleT> {
};

/**
 * Data structure to store the active data members
 */
template <typename EleT>
struct ActiveStore {
    Node<EleT>*   m_node {nullptr};
    int32_t m_strId {0};
    int32_t m_length {0};
};
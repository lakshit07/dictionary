#pragma once

#include <cstdint>
#include <unordered_map>
#include <unordered_set>

// Forward declaration
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

struct Transition {
    EdgeString m_edgeStr;
    Node* m_next;

    Transition() : m_next(nullptr) {}
    Transition(const EdgeString &mEdgeStr, Node *mNext) : m_edgeStr(mEdgeStr),
                                                          m_next(mNext) {}
};

/**
 * Node representation of the suffix tree
 */
struct Node {
    std::unordered_map<char, Transition> m_transitionMap;
    std::unordered_set<int32_t> m_stringIds;
    Node* m_suffixLink;

    Node() : m_suffixLink(nullptr) {}

    virtual Transition next(char ch) {
        if (m_transitionMap.find(ch) == m_transitionMap.end()) {
            return {{0, 0, -1}, nullptr};
        }
        return m_transitionMap[ch];
    }
};

struct Sink : Node {
    Transition next(char ch) override {
        return Transition({0, 0, 0}, this->m_suffixLink);
    }
};

struct Leaf : Node {
    int32_t m_stringId;
    int32_t m_position;
};

/**
 * Data structure to store the active data members
 */
struct ActiveStore {
    Node*   m_node {nullptr};
    int32_t m_edge {0};
    int32_t m_length {0};
};
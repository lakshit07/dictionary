#pragma once

#include <cstdint>
#include <unordered_map>
#include <unordered_set>

// Forward declaration
struct Node;

/**
 * Edge representation of the suffix tree
 */
struct Edge {
    std::unordered_set<std::string>::const_iterator m_string;
    int32_t m_left, m_right;
    Node*   m_next;

    Edge() : m_left(0), m_right(0), m_next(nullptr) {}

    Edge(std::unordered_set<std::string>::const_iterator string, uint32_t left, uint32_t right, Node* next) :
        m_string(string), m_left(left), m_right(right), m_next(next) {}
};

/**
 * Node representation of the suffix tree
 */
struct Node {
    std::unordered_map<char, Edge> m_transitionMap;
    Node* m_suffixLink;

    Node() : m_suffixLink(nullptr) {}

    std::optional<Edge> next(char ch) {
        if (m_transitionMap.find(ch) == m_transitionMap.end()) {
            return std::nullopt;
        }
        return m_transitionMap[ch];
    }
};

/**
 * Data structure to store the active data members
 */
struct ActiveStore {
    Node*   m_node {nullptr};
    int32_t m_edge {0};
    int32_t m_length {0};
};
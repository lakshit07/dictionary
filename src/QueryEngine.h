#pragma once

#include <src/SuffixTree.h>

#include <iostream>
#include <fstream>

/***
 * Query Engine loads the dictionary word file and then answers the queries based on them
 */
class QueryEngine {

    /**
     * Structure to represent all types of queries
     */
    struct Query {

        enum class QueryType {
            SEARCH,
            COUNT,
            MALFORMED,
        };

        enum class QueryPosition {
            PREFIX,
            SUFFIX,
            SUBSTRING,
            MALFORMED,
        };

        QueryType       m_qType;
        QueryPosition   m_qPos;
        std::string     m_word;

        Query(QueryType qType, QueryPosition qPos, std::string word) : m_qType(qType), m_qPos(qPos),
                                                                       m_word(std::move(word)) {}

        friend std::ostream &operator<<(std::ostream &os, const Query &query) {
            os << "m_qType: " << static_cast<int> (query.m_qType) << " m_qPos: " << static_cast<int> (query.m_qPos)
               << " m_word: " << query.m_word;
            return os;
        }

        [[nodiscard]] bool isValid() const {
            return m_qType != QueryType::MALFORMED and m_qPos != QueryPosition::MALFORMED;
        }
    };

    /**
     * Normalize the word by converting it to lower case and removed non alphabetic characters
     * @param word Input word
     * @return Normalized string
     */
    static std::string sanitize(const std::string& word);

    SuffixTree m_tree;
    std::vector<Query> m_queries;

public:
    /**
     * Processes the command line arguments. Expected format <binary> <wordFile> <queryFile>
     * @param argc number of arguments
     * @param argv array of arguments
     */
    bool init(int argc, const char** argv);

    /***
     * Runs through the queries in the queryFile and processes them
     */
    void process(std::ostream& outFile = std::cout) const;

};

#include <src/QueryEngine.hpp>

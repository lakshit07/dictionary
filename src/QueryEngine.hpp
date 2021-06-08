#include <fstream>
#include <iostream>
#include <utility>

std::string QueryEngine::sanitize(const std::string &word) {
    std::string normalizedWord;
    for (char ch : word) {
        if (!isalnum(ch))
            continue;
        normalizedWord.push_back(tolower(ch));
    }
    return normalizedWord;
}

bool QueryEngine::init(int argc, const char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <path of wordList file> <path to queryList file>" << std::endl;
        return false;
    }

    std::ifstream wordFile(argv[1], std::ifstream::in);
    if (wordFile.fail()) {
        std::cerr << "Failed to open wordFile at " << argv[1];
        return false;
    }

    std::ifstream queryFile(argv[2], std::ifstream::out);
    if (wordFile.fail()) {
        std::cerr << "Failed to open queryFile at " << argv[2] << std::endl;
        return false;
    }

    std::string word;
    while (wordFile >> word) {
        const auto& normWord = sanitize((word));
        if (m_wordList.find(normWord) != m_wordList.end()) {
            continue;
        }
        m_wordList.insert(normWord);
        m_tree.insert(normWord);
    }

    m_tree.populateIndices();

    std::string type;
    uint32_t lineNum = 1;

    Query::QueryType qType;
    Query::QueryPosition qPos;

    while (queryFile >> type >> word) {

        if (type.size() != 2) {
            std::cerr << "Ill-formed query type on line number " << lineNum << std::endl;
            qType = Query::QueryType::MALFORMED;
        }
        else if (word.empty()) {
            std::cerr << "No word to lookup on line number " << word << std::endl;
            qType = Query::QueryType::MALFORMED;
        }
        else {
            switch(type[0]) {
                case 'S' :
                    qType = Query::QueryType::SEARCH;
                    break;
                case 'C' :
                    qType = Query::QueryType::COUNT;
                    break;
                default :
                    std::cerr << "Unrecognized query type " << type[0] << " on line number " << lineNum << std::endl;
                    qType = Query::QueryType::MALFORMED;
            }

            switch(type[1]) {
                case '1' :
                    qPos = Query::QueryPosition::PREFIX;
                    break;
                case '2' :
                    qPos = Query::QueryPosition::SUFFIX;
                    break;
                case '3' :
                case '4' :
                    qPos = Query::QueryPosition::SUBSTRING;
                    break;
                default :
                    std::cerr << "Unrecognized query position " << type[1] << " on line number " << lineNum << std::endl;
                    qPos = Query::QueryPosition::MALFORMED;
            }
        }

        m_queries.emplace_back(qType, qPos, word);
        lineNum++;
    }

    return true;
}

void QueryEngine::process(std::ostream& outFile) {
    for (const auto& query : m_queries) {
        if (!query.isValid()) {
            outFile << std::endl;
        }
        else {
            if (query.m_qType == Query::QueryType::SEARCH) {
                std::vector<std::string> result;
                switch (query.m_qPos) {
                    case Query::QueryPosition::PREFIX :
                        result = m_tree.searchPrefix(query.m_word);
                        break;
                    case Query::QueryPosition::SUFFIX :
                        result = m_tree.searchSuffix(query.m_word);
                        break;
                    case Query::QueryPosition::SUBSTRING :
                        result = m_tree.searchSubstring(query.m_word);
                        break;
                    case Query::QueryPosition::MALFORMED:
                    default:
                        result = {};
                }

                for (const auto& val : result) {
                    outFile << val << " ";
                }
                outFile << std::endl;
            }
            else {
                uint32_t count = 0;
                switch (query.m_qPos) {
                    case Query::QueryPosition::PREFIX :
                        count = m_tree.countPrefix(query.m_word);
                        break;
                    case Query::QueryPosition::SUFFIX :
                        count = m_tree.countSuffix(query.m_word);
                        break;
                    case Query::QueryPosition::SUBSTRING :
                        count = m_tree.countSubstring(query.m_word);
                        break;
                    case Query::QueryPosition::MALFORMED:
                    default:
                        count = 0;
                }

                outFile << count << std::endl;
            }
        }
    }
}
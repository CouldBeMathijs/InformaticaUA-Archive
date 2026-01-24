
#include "TokenMatcher.h"
#include <iostream>
#include <sstream>
#include <regex>
#include <filesystem>

void TokenMatcher::loadLanguageDFAs(const std::string& folder) {
    namespace fs = std::filesystem;
    for (const auto& file : fs::directory_iterator(folder)) {
        if (file.path().extension() == ".json") {
            std::string name = file.path().stem().string();
            try {
                dfaMap[name] = Norkella::DFA(file.path().string());
                std::cout << "Loaded DFA for: " << name << "\n";
            } catch (...) {
                std::cerr << "Failed to load DFA for: " << name << "\n";
            }
        }
    }
}

std::vector<std::string> TokenMatcher::tokenize(const std::string& line) const {
    std::vector<std::string> tokens;
    std::regex wordRegex(R"([^\s]+)");
    auto wordsBegin = std::sregex_iterator(line.begin(), line.end(), wordRegex);
    auto wordsEnd = std::sregex_iterator();
    for (std::sregex_iterator i = wordsBegin; i != wordsEnd; ++i)
        tokens.push_back(i->str());
    return tokens;
}
// Analyze text to determine which DFA (language) each token belongs to
void TokenMatcher::analyzeText(const std::string& inputText) {
    tokenMatches.clear();
    totalTokens = 0;

    std::istringstream stream(inputText);
    std::string line;
    std::vector<std::string> allTokens;

    // Tokenize the full input text line by line
    while (std::getline(stream, line)) {
        auto tokens = tokenize(line);
        allTokens.insert(allTokens.end(), tokens.begin(), tokens.end());
    }

    totalTokens = allTokens.size();
    int windowSize = 5;

    for (size_t i = 0; i < allTokens.size(); ++i) {
        std::string token = " " + allTokens[i] + " ";
        if (ignoreCase) {
            std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        }

        std::map<std::string, int> windowMatches;
        std::vector<std::string> acceptedDFAs;

        int halfWin = windowSize / 2;
        int start = std::max<int>(0, i - halfWin);
        int end = std::min<int>(allTokens.size(), i + halfWin + 1);

        for (int j = start; j < end; ++j) {
            std::string contextToken = " " + allTokens[j] + " ";
            if (ignoreCase) {
                std::transform(contextToken.begin(), contextToken.end(), contextToken.begin(), ::tolower);
            }

            for (auto& [lang, dfa] : dfaMap) {
                if (dfa.accepts(contextToken)) {
                    windowMatches[lang]++;
                }
            }
        }

        // See which DFAs accept the current token
        for (auto& [lang, dfa] : dfaMap) {
            if (dfa.accepts(token)) {
                acceptedDFAs.push_back(lang);
            }
        }

        // If only one DFA accepts it, assign it confidently
        if (acceptedDFAs.size() == 1) {
            tokenMatches[acceptedDFAs[0]] += 1.5;
        } else {// Otherwise use window context to guess the most likely language
            std::string bestLang;
            int bestCount = 0;
            for (const auto& [lang, count] : windowMatches) {
                if (count > bestCount) {
                    bestLang = lang;
                    bestCount = count;
                }
            }

            if (!bestLang.empty() && dfaMap[bestLang].accepts(token)) {
                tokenMatches[bestLang]++;
            }
        }
    }
}
// Print a summary report of the language match results
void TokenMatcher::printReport() const {
    std::cout << "\n=== Language Match Report ===\n";
    std::cout << "Total number of tokens:" << totalTokens << "\n";

    if (totalTokens == 0) {
        std::cout << "No tokens to analyze.\n";
        return;
    }

    if (tokenMatches.empty()) {
        std::cout << "No language recognized any tokens.\n";
        return;
    }

    std::string bestLang;
    int bestScore = 0;

    for (const auto& [lang, count] : tokenMatches) {
        if (count > bestScore) {
            bestScore = count;
            bestLang = lang;
        }
    }

    std::cout << "\nBest match: " << bestLang << " (" << bestScore << " matches)\n\n";

    for (const auto& [lang, count] : tokenMatches) {
        double percent = (100.0 * count) / totalTokens;
        std::cout << lang << ": " << std::fixed << std::setprecision(2)
                  << percent << "% (" << count << " tokens accepted)\n";
    }
}

std::map<std::string, double> TokenMatcher::getScores() const {
    std::map<std::string, double> scores;
    for (const auto& [lang, count] : tokenMatches) {
        if (totalTokens > 0)
            scores[lang] = count;
    }
    return scores;
}

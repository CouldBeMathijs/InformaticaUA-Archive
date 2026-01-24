#ifndef TOKANIZE_H
#define TOKANIZE_H

#include "../config/OperatorConfig.h"

#include <string>
#include <vector>

namespace Tokenize {

/**
 * @file Tokenizer.h
 * @brief Shared tokenizer for CYK and (later) the SLR parser.
 *
 * Output tokens:
 *   NUM, ID, OP_BIN, OP_UN, FUNC1, FUNC2, (, ), ,
 *
 * Spaces are allowed but not required.
 *
 * Example:
 *   "5 $ 6 + abs(3)"  ->  NUM OP_BIN NUM OP_BIN FUNC1 ( NUM )
 *
 * @param input Raw formula string.
 * @param ops Operator configuration (from operators.json).
 * @return Vector of CYK grammar terminals as strings.
 */
std::vector<std::string> tokenizeToCykSymbols(const std::string&         input,
                                              const cfg::OperatorConfig& ops);

/**
 * @brief Tokenize input into raw lexemes for the SLR parser.
 *
 * Output tokens are "raw" symbols/lexemes:
 *   - numbers are kept as literals ("12.5")
 *   - identifiers are kept ("x", "abs", "baba")
 *   - operators are kept ("+", "$", "**", ...)
 *   - punctuation: "(", ")", ","
 *
 * Spaces are allowed but not required.
 *
 * @param input Raw formula string.
 * @param ops Operator configuration (used for longest-match on operators).
 * @return Vector of raw tokens for the parser.
 */
std::vector<std::string> tokenizeForParser(const std::string&         input,
                                           const cfg::OperatorConfig& ops);

} // namespace Tokenize
#endif

#include "OperatorConfig.h"

#include "../external/json.hpp"

#include <fstream>
#include <stdexcept>
using json = nlohmann::json;

namespace cfg {

static std::string getNotationOrDefault(const json& def, const std::string& fallback) {
    if (def.is_object() && def.contains("notation") && def["notation"].is_string())
        return def["notation"].get<std::string>();
    return fallback;
}

static int getArityOrDefault(const json& def, int fallback) {
    if (def.is_object() && def.contains("arity") && def["arity"].is_number_integer())
        return def["arity"].get<int>();
    return fallback;
}

static int getWeightOrDefault(const json& def, const int fallback) {
    if (def.is_object() && def.contains("weight") && def["weight"].is_number_integer())
        return def["weight"].get<int>();
    return fallback;
}

static bool getRightAssocOrDefault(const json& def, const bool fallback) {
    if (def.is_object() && def.contains("assoc") && def["assoc"].is_string()) {
        std::string a = def["assoc"].get<std::string>();
        return (a == "right");
    }
    return fallback;
}

static void insertFromUnaryArray(OperatorConfig& cfgObj, const json& arr) {
    if (!arr.is_array())
        return;

    for (const auto& el : arr) {
        std::string sym;
        if (el.is_string()) {
            sym = el.get<std::string>();
            cfgObj.un.insert(sym);
            continue;
        }
        if (!el.is_object() || !el.contains("symbol") || !el["symbol"].is_string())
            continue;

        sym                        = el["symbol"].get<std::string>();
        const std::string notation = getNotationOrDefault(el, "prefix");

        if (notation == "call") {
            const int arity = getArityOrDefault(el, 1);
            if (arity == 1)
                cfgObj.func1.insert(sym);
            else if (arity == 2)
                cfgObj.func2.insert(sym);
            else {
                // unsupported arity -> ignore for syntax classification
            }
        } else if (notation == "postfix") {
            cfgObj.post.insert(sym);
        } else {
            cfgObj.un.insert(sym);

            cfgObj.binMeta[sym] = BinOpMeta{.weight     = getWeightOrDefault(el, 0),
                                            .rightAssoc = getRightAssocOrDefault(el, false)};
        }
    }
}

static void insertFromBinaryArray(OperatorConfig& cfgObj, const json& arr) {
    if (!arr.is_array())
        return;

    for (const auto& el : arr) {
        std::string sym;
        if (el.is_string()) {
            sym = el.get<std::string>();
            cfgObj.bin.insert(sym);
            continue;
        }
        if (!el.is_object() || !el.contains("symbol") || !el["symbol"].is_string())
            continue;

        sym                        = el["symbol"].get<std::string>();
        const std::string notation = getNotationOrDefault(el, "infix");

        if (notation == "call") {
            const int arity = getArityOrDefault(el, 2);
            if (arity == 1)
                cfgObj.func1.insert(sym);
            else if (arity == 2)
                cfgObj.func2.insert(sym);
        } else {
            cfgObj.bin.insert(sym);

            cfgObj.binMeta[sym] = BinOpMeta{.weight     = getWeightOrDefault(el, 0),
                                            .rightAssoc = getRightAssocOrDefault(el, false)};
        }
    }
}

OperatorConfig::OperatorConfig(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open())
        throw std::runtime_error("Cannot open operators file: " + filename);

    json j;
    f >> j;

    if (j.contains("unary_operators"))
        insertFromUnaryArray(*this, j["unary_operators"]);
    if (j.contains("binary_operators"))
        insertFromBinaryArray(*this, j["binary_operators"]);

    // Backwards-compatible optional keys (if you still want them):
    if (j.contains("func1_operators")) {
        const auto& a = j["func1_operators"];
        if (a.is_array()) {
            for (const auto& el : a)
                if (el.is_string())
                    func1.insert(el.get<std::string>());
                else if (el.is_object() && el.contains("symbol") && el["symbol"].is_string())
                    func1.insert(el["symbol"].get<std::string>());
        }
    }
    if (j.contains("func2_operators")) {
        const auto& a = j["func2_operators"];
        if (a.is_array()) {
            for (const auto& el : a)
                if (el.is_string())
                    func2.insert(el.get<std::string>());
                else if (el.is_object() && el.contains("symbol") && el["symbol"].is_string())
                    func2.insert(el["symbol"].get<std::string>());
        }
    }

    all.clear();
    all.insert(bin.begin(), bin.end());
    all.insert(un.begin(), un.end());
    all.insert(post.begin(), post.end());
    all.insert(func1.begin(), func1.end());
    all.insert(func2.begin(), func2.end());
}

bool OperatorConfig::isBinary(const std::string& lexeme) const { return bin.contains(lexeme); }

bool OperatorConfig::isUnary(const std::string& lexeme) const { return un.contains(lexeme); }

bool OperatorConfig::isFunc1(const std::string& lexeme) const { return func1.contains(lexeme); }

bool OperatorConfig::isFunc2(const std::string& lexeme) const { return func2.contains(lexeme); }

const std::set<std::string>& OperatorConfig::allOperatorLexemes() const { return all; }

int                          OperatorConfig::binaryWeight(const std::string& lexeme) const {
    const auto it = binMeta.find(lexeme);
    return (it == binMeta.end()) ? 0 : it->second.weight;
}

bool OperatorConfig::binaryRightAssoc(const std::string& lexeme) const {
    const auto it = binMeta.find(lexeme);
    return it != binMeta.end() && it->second.rightAssoc;
}

bool OperatorConfig::isPostfix(const std::string& lexeme) const { return post.contains(lexeme); }

} // namespace cfg

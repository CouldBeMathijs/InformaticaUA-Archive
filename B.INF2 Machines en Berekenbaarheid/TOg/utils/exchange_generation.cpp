// This file is not part of the main build, it is used to fetch recent exchange rates for
// operators.json Build manually with g++ -std=c++11 exchange_generation.cpp -o operatorgen -lcurl

#include "../external/json.hpp"

#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

json fetch_exchange_rates() {
    CURL*       curl;
    CURLcode    res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.frankfurter.app/latest");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK)
            throw std::runtime_error("CURL request failed");
    }
    return json::parse(readBuffer);
}

int main() {
    try {
        json output;

        // 1. Fetch API Data first to get the timestamp
        std::cout << "Fetching currency data..." << std::endl;
        json rateData = fetch_exchange_rates();

        // 2. Add Metadata
        output["metadata"] = {
            {"source", "Frankfurter API"},
            {"api_date", rateData["date"]},         // This is the YYYY-MM-DD from the API
            {"generated_at", __DATE__ " " __TIME__} // Local compilation timestamp
        };

        // 3. Static Unary Operators
        output["unary_operators"] = {
            {{"symbol", "+"},
             {"notation", "prefix"},
             {"expr",
              {{"type", "unary"}, {"op", "pos"}, {"arg", {{"type", "var"}, {"name", "x"}}}}}},
            {{"symbol", "-"},
             {"notation", "prefix"},
             {"expr",
              {{"type", "unary"}, {"op", "neg"}, {"arg", {{"type", "var"}, {"name", "x"}}}}}},
            {{"symbol", "abs"},
             {"notation", "call"},
             {"arity", 1},
             {"expr",
              {{"type", "unary"}, {"op", "abs"}, {"arg", {{"type", "var"}, {"name", "x"}}}}}},
            {{"symbol", "sqrt"},
             {"notation", "call"},
             {"arity", 1},
             {"expr",
              {{"type", "unary"}, {"op", "sqrt"}, {"arg", {{"type", "var"}, {"name", "x"}}}}}},
            {{"symbol", "log"},
             {"notation", "call"},
             {"arity", 1},
             {"expr",
              {{"type", "unary"}, {"op", "log"}, {"arg", {{"type", "var"}, {"name", "x"}}}}}},
            {{"symbol", "sin"},
             {"notation", "call"},
             {"arity", 1},
             {"expr",
              {{"type", "unary"}, {"op", "sin"}, {"arg", {{"type", "var"}, {"name", "x"}}}}}},
            {{"symbol", "cos"},
             {"notation", "call"},
             {"arity", 1},
             {"expr",
              {{"type", "unary"}, {"op", "cos"}, {"arg", {{"type", "var"}, {"name", "x"}}}}}},
            {{"symbol", "tan"},
             {"notation", "call"},
             {"arity", 1},
             {"expr",
              {{"type", "unary"}, {"op", "tan"}, {"arg", {{"type", "var"}, {"name", "x"}}}}}},
            {{"symbol", "%"},
             {"notation", "postfix"},
             {"expr",
              {{"type", "binary"},
               {"op", "div"},
               {"left", {{"type", "var"}, {"name", "x"}}},
               {"right", {{"type", "number"}, {"value", 100}}}}}}};

        // 4. Static Binary Operators
        output["binary_operators"] = {{{"symbol", "+"},
                                       {"notation", "infix"},
                                       {"weight", 10},
                                       {"assoc", "left"},
                                       {"expr",
                                        {{"type", "binary"},
                                         {"op", "add"},
                                         {"left", {{"type", "var"}, {"name", "a"}}},
                                         {"right", {{"type", "var"}, {"name", "b"}}}}}},
                                      {{"symbol", "-"},
                                       {"notation", "infix"},
                                       {"weight", 10},
                                       {"assoc", "left"},
                                       {"expr",
                                        {{"type", "binary"},
                                         {"op", "sub"},
                                         {"left", {{"type", "var"}, {"name", "a"}}},
                                         {"right", {{"type", "var"}, {"name", "b"}}}}}},
                                      {{"symbol", "*"},
                                       {"notation", "infix"},
                                       {"weight", 20},
                                       {"assoc", "left"},
                                       {"expr",
                                        {{"type", "binary"},
                                         {"op", "mul"},
                                         {"left", {{"type", "var"}, {"name", "a"}}},
                                         {"right", {{"type", "var"}, {"name", "b"}}}}}},
                                      {{"symbol", "/"},
                                       {"notation", "infix"},
                                       {"weight", 20},
                                       {"assoc", "left"},
                                       {"expr",
                                        {{"type", "binary"},
                                         {"op", "div"},
                                         {"left", {{"type", "var"}, {"name", "a"}}},
                                         {"right", {{"type", "var"}, {"name", "b"}}}}}},
                                      {{"symbol", "^"},
                                       {"notation", "infix"},
                                       {"weight", 30},
                                       {"assoc", "right"},
                                       {"expr",
                                        {{"type", "binary"},
                                         {"op", "pow"},
                                         {"left", {{"type", "var"}, {"name", "a"}}},
                                         {"right", {{"type", "var"}, {"name", "b"}}}}}},
                                      {{"symbol", "min"},
                                       {"notation", "call"},
                                       {"arity", 2},
                                       {"expr",
                                        {{"type", "binary"},
                                         {"op", "min"},
                                         {"left", {{"type", "var"}, {"name", "a"}}},
                                         {"right", {{"type", "var"}, {"name", "b"}}}}}},
                                      {{"symbol", "max"},
                                       {"notation", "call"},
                                       {"arity", 2},
                                       {"expr",
                                        {{"type", "binary"},
                                         {"op", "max"},
                                         {"left", {{"type", "var"}, {"name", "a"}}},
                                         {"right", {{"type", "var"}, {"name", "b"}}}}}},
                                      {{"symbol", "atan2"},
                                       {"notation", "call"},
                                       {"arity", 2},
                                       {"expr",
                                        {{"type", "binary"},
                                         {"op", "atan2"},
                                         {"left", {{"type", "var"}, {"name", "a"}}},
                                         {"right", {{"type", "var"}, {"name", "b"}}}}}}};

        // 5. Inject Currency Postfix Operators
        json rates   = rateData["rates"];
        rates["EUR"] = 1;
        std::cout << "Loaded " << rates.size() << " currencies..." << std::endl;
        // 5.5 Sort Unary Operators by Symbol
        std::sort(output["unary_operators"].begin(), output["unary_operators"].end(),
                  [](const json& a, const json& b) {
                      return a["symbol"].get<std::string>() < b["symbol"].get<std::string>();
                  });
        for (auto it = rates.begin(); it != rates.end(); ++it) {
            output["unary_operators"].push_back(
                {{"symbol", it.key()},
                 {"notation", "postfix"},
                 {"expr",
                  {{"type", "binary"},
                   {"op", "div"},
                   {"left", {{"type", "var"}, {"name", "x"}}},
                   {"right", {{"type", "number"}, {"value", it.value()}}}}}});
        }

        // 6. Write to File
        std::ofstream file("operators.json");
        file << output.dump(4, ' ', false, nlohmann::json::error_handler_t::replace);
        std::cout << "File 'operators.json' updated with API timestamp: " << rateData["date"]
                  << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

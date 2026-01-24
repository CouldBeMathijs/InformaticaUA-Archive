#ifndef PROGRAMMEEROPDRACHT_CFG_H
#define PROGRAMMEEROPDRACHT_CFG_H
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class CFG {
private:
    std::set<std::string> variables;
    std::set<std::string> terminals;
    std::unordered_map<std::string, std::vector<std::vector<std::string> > > productions;
    std::string startsymbol;

public:
    explicit CFG(const std::string &);

    CFG() = default;

    CFG(const std::set<std::string> &variables, const std::set<std::string> &terminals,
        const std::unordered_map<std::string, std::vector<std::vector<std::string> > > &productions,
        const std::string &startsymbol);

    std::string to_string() const;

    void print() const;

    bool accepts(const std::string& in) const;
};

class table
{
private:
    std::map<std::pair<size_t, size_t>, std::set<std::string>> data;
    size_t size_n;

    bool is_lower_triangular(size_t row, size_t col) const;

public:
    explicit table(size_t n);

    std::set<std::string>& get(size_t row, size_t col);

    void set(size_t row, size_t col, const std::set<std::string>& value);

    size_t calculateColWidth(size_t col);

    static std::string subStringCell(const std::set<std::string>& s, size_t requested_size);

    std::string to_string();

    void print();
};


#endif //PROGRAMMEEROPDRACHT_CFG_H

#include "CFG.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include "json.hpp"


CFG::CFG(const std::string& file)
{
    std::ifstream input(file);
    if (!input)
    {
        throw std::invalid_argument("File " + file + " not found");
    }
    using json = nlohmann::json;
    json j;
    input >> j;

    for (const auto& var : j["Variables"])
    {
        if (var.is_string())
        {
            variables.insert(var.get<std::string>());
        }
    }

    for (const auto& term : j["Terminals"])
    {
        if (term.is_string())
        {
            terminals.insert(term.get<std::string>());
        }
    }
    for (const auto& prod_obj : j["Productions"])
    {
        if (prod_obj.is_object() && prod_obj.contains("head") && prod_obj.contains("body"))
        {
            auto head = prod_obj["head"].get<std::string>();
            std::vector<std::string> body_vec;
            if (prod_obj["body"].is_array())
            {
                for (const auto& symbol : prod_obj["body"])
                {
                    if (symbol.is_string())
                    {
                        body_vec.push_back(symbol.get<std::string>());
                    }
                }
            }
            productions[head].push_back(body_vec);
        }
    }

    startsymbol = j["Start"].get<std::string>();
}

CFG::CFG(const std::set<std::string>& variables, const std::set<std::string>& terminals,
         const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& productions,
         const std::string& startsymbol) : variables(variables),
                                           terminals(terminals),
                                           productions(productions),
                                           startsymbol(startsymbol)
{
}

std::string CFG::to_string() const
{
    std::stringstream result;

    // Print Variables (V)
    result << "V = {";
    bool first = true;
    for (const auto& var : variables)
    {
        if (!first)
        {
            result << ", ";
        }
        result << var;
        first = false;
    }
    result << "}\n";

    // Print Terminals (T)
    result << "T = {";
    first = true;
    for (const auto& term : terminals)
    {
        if (!first)
        {
            result << ", ";
        }
        result << term;
        first = false;
    }
    result << "}\n";

    result << "P = {\n";

    std::vector<std::string> productionKeys;
    for (const auto& pair : productions)
    {
        productionKeys.push_back(pair.first);
    }
    std::sort(productionKeys.begin(), productionKeys.end());

    for (const auto& key : productionKeys)
    {
        std::vector<std::vector<std::string>> sorted_bodies = productions.at(key);
        std::sort(sorted_bodies.begin(), sorted_bodies.end());
        for (const auto& bodies : sorted_bodies)
        {
            result << "    " << key << " -> `";
            bool first_symbol = true;
            if (bodies.empty())
            {
                result << "";
            }
            else
            {
                for (const auto& symbol : bodies)
                {
                    if (!first_symbol)
                    {
                        result << " ";
                    }
                    result << symbol;
                    first_symbol = false;
                }
            }
            result << "`\n";
        }
    }

    result << "}\n";

    result << "S = " << startsymbol;

    return result.str();
}

void CFG::print() const
{
    std::cout << this->to_string() << std::endl;
}


bool CFG::accepts(const std::string& in) const
{
    if (in.empty())
    {
        return false;
    }

    const size_t& n = in.size();
    // Bottom row, check terminals
    table tabel(n);
    for (size_t i = 0; i < n; ++i)
    {
        std::set<std::string> cell_i;
        std::string terminal_char(1, in[i]);

        for (const auto& pair : productions)
        {
            const std::string& A = pair.first;
            const auto& rhs_list = pair.second;

            for (const auto& rhs : rhs_list)
            {
                // CNF terminal rules are A -> a
                if (rhs.size() == 1 && rhs[0] == terminal_char)
                {
                    cell_i.insert(A);
                }
            }
        }
        tabel.set(n - 1, i, cell_i);
    }

    // Fill in the other cells

    // 'r' is the ROW INDEX. It goes from n-2 (length 2) up to 0 (length n).
    for (int r = n - 2; r >= 0; r--)
    {
        // 'j' is the START INDEX (column in your table).
        // It ranges from 0 up to r (to ensure the substring fits within n)
        for (int j = 0; j <= r; j++)
        {
            // Substring length is L = n - r.

            std::set<std::string> current_cell;

            // 'k' is the SPLIT point. This logic must find previously computed cells.
            // We need to split the substring in[j ... j + L - 1] into two parts.
            // Part 1: Length 's', Row Index 'n - s'
            // Part 2: Length 'L - s', Row Index 'n - (L - s)'

            // 's' is the length of the first part, ranging from 1 to L-1
            int L = n - r;
            for (int s = 1; s < L; s++)
            {
                // Part 1 (B): Length 's'. Row Index: n - s. Start Index: j.
                int r1 = n - s;
                const std::set<std::string>& set1 = tabel.get(r1, j);

                // Part 2 (C): Length 'L - s'. Row Index: n - (L - s). Start Index: j + s.
                int r2 = n - (L - s);
                int j2 = j + s;
                const std::set<std::string>& set2 = tabel.get(r2, j2);

                // Look for rules A -> BC
                for (const std::string& B : set1)
                {
                    for (const std::string& C : set2)
                    {
                        //std::string rhs = B + C;
                        std::vector<std::string> rhs;
                        rhs.push_back(B);
                        rhs.push_back(C);

                        // Iterate over all productions
                        for (const auto& pair : productions)
                        {
                            const std::string& A = pair.first;
                            const auto& rhs_list = pair.second;

                            for (const auto& prod_rhs : rhs_list)
                            {
                                if (prod_rhs == rhs)
                                {
                                    current_cell.insert(A);
                                }
                            }
                        }
                    }
                }
            }
            // Set the calculated set for the current cell V[r][j]
            tabel.set(r, j, current_cell);
        }
    }

    tabel.print();
    const std::set<std::string>& final_set = tabel.get(0, 0);
    bool result = false;
    for (const auto& symbol : final_set)
    {
        if (symbol == startsymbol)
        {
            result = true;
        }
    }
    if (result)
    {
        std::cout << "true" << std::endl;
    }
    else
    {
        std::cout << "false" << std::endl;
    }

    return result;
}


bool table::is_lower_triangular(const size_t row, const size_t col) const
{
    return row < size_n && col < size_n && row >= col;
}

table::table(const size_t n) : size_n(n)
{
    for (size_t i = 0; i < size_n; i++)
    {
        for (size_t j = 0; j <= i; j++)
        {
            data[std::make_pair(i, j)] = {};
        }
    }
}

std::set<std::string>& table::get(size_t row, size_t col)
{
    if (!is_lower_triangular(row, col))
    {
        if (row < col)
        {
            throw std::out_of_range(
                "Cell indices (" + std::to_string(row) + " " + std::to_string(col) +
                ") are out of bounds, only lower triangle built");
        }
        throw std::out_of_range("Cell indices are out of bounds.");
    }
    return data.at(std::make_pair(row, col));
}

void table::set(size_t row, size_t col, const std::set<std::string>& value)
{
    if (!is_lower_triangular(row, col))
    {
        if (row < col)
        {
            throw std::out_of_range(
                "Cell indices (" + std::to_string(row) + " " + std::to_string(col) +
                ") are out of bounds, only lower triangle built");
        }
        throw std::out_of_range("Cell indices are out of bounds.");
    }

    data[std::make_pair(row, col)] = value;
}

size_t table::calculateColWidth(const size_t col)
{
    size_t max = 0;
    if (col >= size_n)
    {
        throw std::out_of_range("Col must be between 0 and " + std::to_string(size_n - 1));
    }
    for (size_t i = 0; i < size_n; i++)
    {
        try
        {
            max = std::max(get(i, col).size(), max);
        }
        catch (std::exception& e)
        {
        }
    }
    return max;
}

std::string table::subStringCell(const std::set<std::string>& s, const size_t requested_size)
{
    std::stringstream content_ss;
    for (auto it = s.begin(); it != s.end(); ++it)
    {
        content_ss << *it;
        if (std::next(it) != s.end())
        {
            content_ss << ", ";
        }
    }
    const std::string content = content_ss.str();

    const size_t required_content_size = content.length() + 2;

    if (required_content_size > requested_size)
    {
        throw std::runtime_error("Required string content size (" +
            std::to_string(required_content_size) +
            ") exceeds requested size (" +
            std::to_string(requested_size) + ")");
    }

    const size_t padding_needed = requested_size - required_content_size;

    std::stringstream final_ss;
    final_ss << "{";
    final_ss << content;
    final_ss << "}";

    final_ss << std::string(padding_needed, ' ');
    return final_ss.str();
}

std::string table::to_string()
{
    std::stringstream ss;

    std::vector<size_t> col_widths(size_n);
    for (size_t j = 0; j < size_n; ++j)
    {
        col_widths[j] = 2 + 3 * this->calculateColWidth(j);
    }
    for (size_t i = 0; i < size_n; ++i)
    {
        for (size_t j = 0; j <= i; ++j)
        {
            std::set<std::string> cell_data = this->get(i, j);

            const size_t& width = col_widths[j];

            std::string cell_str = subStringCell(cell_data, width);

            ss << "| " << cell_str << "  ";
        }
        ss << "|";
        if (i < size_n - 1)
        ss << "\n";
    }

    return ss.str();
}

void table::print()
{
    std::cout << this->to_string() << std::endl;
}

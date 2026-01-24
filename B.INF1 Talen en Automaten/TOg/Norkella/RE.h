#ifndef RE_NORAN_H
#define RE_NORAN_H

#include <string>
namespace Norkella {
class RE {
private:
    std::string expression;

public:
    RE();
    explicit RE(const std::string& expr);

    void print() const;
    [[nodiscard]] const std::string& str() const;

    static RE concat(const RE& a, const RE& b);
    static RE unionRE(const RE& a, const RE& b);
    static RE star(const RE& a);
};
}

#endif // RE_NORAN_H


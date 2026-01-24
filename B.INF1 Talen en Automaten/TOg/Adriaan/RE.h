#ifndef RE_ADRIAAN_H
#define RE_ADRIAAN_H

#include "ENFA.h"
#include <string>
#include "../Norkella/RE.h"
namespace Adriaan {
class RE {
private:
  std::string expression;
  char epsilon;

public:
  RE(const std::string &expr, char eps);

  static bool isLiteral(char c);

  [[nodiscard]] ENFA toENFA() const;

  [[nodiscard]] std::string getExpression() const;

  [[nodiscard]] char getEpsilon() const;
};
}
#endif
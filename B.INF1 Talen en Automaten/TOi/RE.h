#ifndef RE_H
#define RE_H
#include "ENFA.h"

#include <string>

class RE {
private:
  std::string regex;
  char epsilon;
public:
  RE(const std::string &regex, char epsilon);
  ENFA toENFA() const;
};



#endif //RE_H

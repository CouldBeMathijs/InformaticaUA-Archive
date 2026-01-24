#ifndef LANGUAGES_H
#define LANGUAGES_H

#include "Adriaan/RE.h"
#include "Norkella/DFA.h"

#include <string>

namespace Languages {
  void exportToImage(const std::map<std::string, double> &data,
                     const std::string &outputImage);
  void generateImages(const std::string &extension);
  void generateAll();
  void generateLanguages();
  std::string escapeSpecialChars(const std::string &input);
  std::string escapeNonAscii(const std::string &input);
  Adriaan::RE fileToRegex(const std::string &filename) ;
  Norkella::DFA regexToDFA(const Adriaan::RE& in);
  Norkella::DFA fileToDFA(const std::string &filename);
};

#endif // LANGUAGES_H

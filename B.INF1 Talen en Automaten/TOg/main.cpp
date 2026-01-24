#include "Adriaan/RE.h"
#include "Norkella/ENFA.h"
#include "Norkella/DFA.h"
#include "Languages.h"
#include "Gui/Application.h"

int tests() {
  Adriaan::RE test("(abc*a)*", '\?');
  auto aenfa = test.toENFA();
  Norkella::ENFA enfa(aenfa);
  Norkella::DFA dfa = enfa.toDFA(true);
  dfa = dfa.minimize();
  int errors = 0;
  if (!dfa.accepts("aba")) errors++;
  if (!dfa.accepts("abca")) errors++;
  if (!dfa.accepts("abcca")) errors++;
  if (dfa.accepts("abqa")) errors++;
  if (!dfa.accepts("")) errors++;
  if (!dfa.accepts("abaabca")) errors++;
  dfa = Languages::regexToDFA(test);
  if (!dfa.accepts("aba")) errors++;
  if (!dfa.accepts("abca")) errors++;
  if (!dfa.accepts("abcca")) errors++;
  if (dfa.accepts("abqa")) errors++;
  if (!dfa.accepts("")) errors++;
  if (!dfa.accepts("abaabca")) errors++;
  dfa = Norkella::DFA("languages/Dutch.json");
  if (!dfa.accepts(" ik ")) errors++;
  if (dfa.accepts(Languages::escapeNonAscii(" étaient "))) errors++;
  dfa = Norkella::DFA("languages/French.json");
  if (!dfa.accepts(Languages::escapeNonAscii(" étaient "))) errors++;
  dfa = Norkella::DFA("languages/Ukrainian.json");
  if (dfa.accepts(Languages::escapeNonAscii(" étaient "))) errors++;
  if (!dfa.accepts(Languages::escapeNonAscii(" було "))) errors++;
  dfa = Norkella::DFA ("languages/Dutch.json");
  if (!dfa.accepts(" dit ")) errors++;
  if (!dfa.accepts(" is ")) errors++;
  if (!dfa.accepts(" een ")) errors++;
  if (!dfa.accepts(" tekst ")) errors++;
  return errors;
}
int main() {
    Languages::generateAll();
    auto testresult = tests();
      if (testresult != 0) {
    return testresult;
    }
    Application app;
    app.run();
    return 0;
}
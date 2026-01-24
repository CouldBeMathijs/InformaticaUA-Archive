#include <iostream>

#include "PDA.h"

using namespace std;

int main2() {
    PDA pda("/home/mathijs/Documents/UAntwerpen/B.INF2/MB/Programmeeropdracht/input-pda2cfg1.json");
    pda.print();
    pda.toCFG().print();
    return 0;
}
int main() {
    CFG cfg("input-cyk1.json");
    cfg.print();
    cfg.accepts("baaba");
    cfg.accepts("abba");
    return 0;
}
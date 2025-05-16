#include "DFA.h"
#include "ENFA.h"
#include "NFA.h"
#include "RE.h"

#include <iostream>
using namespace std;

int main() {
/*
	cout << "\n## NFA -> DFA using Subset Construction\n" << endl;
	NFA nfa("NFA.json");
	nfa.toDFA().print();

	cout << "\n## ENFA -> DFA using Modified Subset Construction\n" << endl;
	ENFA enfa("ENFA.json");
	enfa.toDFA().print();

	cout << "\n## Products of DFA's\n" << endl;
	DFA dfa1("DFAprod1.json");
	DFA dfa2("DFAprod1.json");
	DFA doorsnede(dfa1,dfa2,true); // true betekent doorsnede, false betekent unie
	doorsnede.print();
	DFA unie(dfa1,dfa2,false);
	unie.print();

/*
	cout << "\n## RE -> ENFA using Thompson's Algorithm\n" << endl;
	RE re("(m+y)*+(e+y+m+i)s",'e');
	ENFA oldre = re.toENFA();
	oldre.printStats();
	// should give true
	cout << boolalpha << oldre.accepts("ys") << endl;
	cout << boolalpha << oldre.accepts("mmyyymmmym") << endl;
	cout << boolalpha << oldre.accepts("s") << endl;

	// should give false
	cout << boolalpha << oldre.accepts("ss") << endl;
	cout << boolalpha << oldre.accepts("ims") << endl;
	cout << boolalpha << oldre.accepts("mimis") << endl;
*/

	cout << "\n## Equivalence and minimalization of DFA's\n" << endl;
	DFA dfa("TFADFA.json");
	dfa.printTransitionTable();
	cout << "\n\nMinimizing dfa" << endl;
	 DFA mindfa = dfa.minimize();
	cout << "\n\nThe minimized DFA looks like this:";
	mindfa.printTransitionTable();
	 // dfa.printTable();
	 cout << endl;
	 mindfa.print();
	//cout << boolalpha << (dfa == mindfa) << endl;
	 cout << "\n Testing and stuff" << endl;
	 DFA testdfa("testDFA.json");
	 testdfa.printTransitionTable();
	 DFA zero = testdfa.minimize();
	return 0;
}

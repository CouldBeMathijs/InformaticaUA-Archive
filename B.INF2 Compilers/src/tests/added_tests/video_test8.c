// test8_optimizations.c
#include <stdio.h>

int main() {
    /* 1. Unused variables (Project 5 - Optioneel)
       De compiler zou geen code moeten genereren voor variabelen die
       wel gedeclareerd zijn, maar nooit worden gebruikt. */
    int unused_variable = 42;
    float another_unused = 3.14;

    /* 2. Conditions that are always false (Project 5 - Optioneel)
       De body van deze if-statement mag niet in de uiteindelijke
       machinecode/LLVM terechtkomen. */
    if (0) {
        printf("Dit wordt nooit geprint.\n");
        int dead_code = 100;
    }

    int counter = 0;
    while (counter < 10) {
        counter++;
        if (counter == 5) {
            continue;
            /* 3. Dead code na een continue/break/return (Project 5 - Verplicht)
               Code na een control-flow onderbreking binnen dezelfde scope
               mag niet gegenereerd worden. */
            printf("Onbereikbare code na continue!\n");
        }

        if (counter == 8) {
            break;
            printf("Onbereikbare code na break!\n"); //
        }
    }

    return 0;

    // Onbereikbare code na de return van de functie
    counter = 999;
}

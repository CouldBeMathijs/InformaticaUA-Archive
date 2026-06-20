// test2_control_flow.c
#include <stdio.h>

enum State {
    START,
    RUNNING,
    STOPPED
};

int main() {
    enum State current_state = START;
    int counter = 0;

    // For loop met if-else en continue/break
    int i;
    for (i = 0; i < 20; i++) {
        if (i % 2 == 0) {
            continue; // Sla even getallen over
        } else if (i == 15) {
            break; // Stop de loop bij 15
        } else {
            counter = counter + i;
        }
    }

    // While loop
    while (counter > 0) {
        counter--;

        // Switch statement
        switch (counter) {
            case 10:
                current_state = RUNNING;
                break;
            case 0:
                current_state = STOPPED;
                break;
            default:
                break;
        }
    }

    // Anonieme scope
    {
        int scoped_var = 99;
        current_state = STOPPED + 1; // Enum constanten gedragen zich als ints
    }

    return 0;
}

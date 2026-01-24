#include "Time.h"


Time :: Time (long s): seconden(s) {
    berekenHMS();
}

string Time::toString() const {
    return to_string(uren) + ": " + to_string(minuten) + ": " + to_string(seconden);
}

void Time::berekenHMS() {
    uren = seconden/3600;
    minuten = seconden/60;
    seconden = seconden % 60;
}


void Time::add(long s) {
    seconden += s;
    berekenHMS();
}
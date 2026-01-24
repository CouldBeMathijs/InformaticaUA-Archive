#ifndef WEEK12_1_TIME_H
#define WEEK12_1_TIME_H

#include <string>

using namespace std;

class Time {
private:
    long seconden;
    long uren;
    long minuten;
    void berekenHMS();
public:
    Time(long s);
    string toString() const;
    void add(long s);
};



#endif //WEEK12_1_TIME_H

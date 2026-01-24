#include <iostream>
#include "DenTweede.h"
#include "DenDerde.h"
#include "EnDeVierde.h"
using namespace std;
class DenEerste: public DenTweede {
public:
    DenEerste(){
        DenDerde d;
        cout << "4";
    }

    virtual ~DenEerste(){
        cout << "6";
    }

private:
    EnDeVierde v;
};

int Oef4() {
    DenEerste d;
    return 0;
}

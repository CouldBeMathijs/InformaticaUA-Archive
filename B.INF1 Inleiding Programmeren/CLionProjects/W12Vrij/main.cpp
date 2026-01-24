#include <iostream>
using namespace std;
#include "Student.h"
#include "Cursus.h"
#include "Professor.h"
#include "Assistent.h"
int Oef1() {
    // student
    Student john("John", "Doe", "12345");
    Student jane("Jane", "Doe", "543210");
    vector<Student*> universiteit;
    universiteit.push_back(&john);
    universiteit.push_back(&jane);

    // course
    Cursus ip("IP", "Inleiding programmeren", 9);
    Cursus gas("GAS", "Gegevensabstractie en -structuren", 6);
    Cursus mc("MC", "Monster Cursus", 56);

    vector<Cursus*> cursussen;
    cursussen.push_back(&ip);
    cursussen.push_back(&gas);
    cursussen.push_back(&mc);

    // inschrijven
    john.schrijfIn(&ip);
    john.schrijfIn(&gas);
    john.schrijfIn(&mc);
    jane.schrijfIn(&ip);

    // print alles
    cout << "Iedereen verbonden aan de universiteit:" << endl;
    for(Student* member : universiteit){
        cout << '\t' << member->toString() << endl;
    }

    cout << "Alle cursussen:" << endl;
    for(Cursus* course : cursussen){
        course->print();
    }

    jane.schrijfUit(&ip);

    // print cursussen
    cout << "Alle cursussen:" << endl;
    for(Cursus* cursus : cursussen){
        cursus->print();
    }
    return 0;
}
int Oef2(){
    // student
    Student john("John", "Doe", "12345");
    Student jane("Jane", "Doe", "543210");
    vector<Student*> universiteit;
    universiteit.push_back(&john);
    universiteit.push_back(&jane);

    // course
    Cursus ip("IP", "Inleiding programmeren", 9);
    Cursus gas("GAS", "Gegevensabstractie en -structuren", 6);
    Cursus mc("MC", "Monster Cursus", 56);

    vector<Cursus*> cursussen;
    cursussen.push_back(&ip);
    cursussen.push_back(&gas);
    cursussen.push_back(&mc);

    // inschrijven
    john.schrijfIn(&ip);
    john.schrijfIn(&gas);
    john.schrijfIn(&mc);
    jane.schrijfIn(&ip);

    // professor
    Professor toon("Toon", "Calders");
    toon.geeft(&ip);
    Professor els("Els", "Laenens");
    els.geeft(&gas);
    vector<Professor*> universiteit2;
    universiteit2.push_back(&toon);
    universiteit2.push_back(&els);

    // print alles
    cout << "Iedereen verbonden aan de universiteit:" << endl;
    for(Student* member : universiteit){
        cout << '\t' << member->toString() << endl;
    }
    for(Professor* member : universiteit2){
        cout << '\t' << member->toString() << endl;
    }

    cout << "Alle cursussen:" << endl;
    for(Cursus* course : cursussen){
        course->print();
    }

    jane.schrijfUit(&ip);

    // print cursussen
    cout << "Alle cursussen:" << endl;
    for(Cursus* cursus : cursussen){
        cursus->print();
    }
    return 0;
}

int Oef4() {
    // student
    Student john("John", "Doe", "12345");
    Student jane("Jane", "Doe", "543210");
    vector<Student*> universiteit;
    universiteit.push_back(&john);
    universiteit.push_back(&jane);

    // course
    Cursus ip("IP", "Inleiding programmeren", 9);
    Cursus gas("GAS", "Gegevensabstractie en -structuren", 6);
    Cursus mc("MC", "Monster Cursus", 56);

    vector<Cursus*> cursussen;
    cursussen.push_back(&ip);
    cursussen.push_back(&gas);
    cursussen.push_back(&mc);

    // inschrijven
    john.schrijfIn(&ip);
    john.schrijfIn(&gas);
    john.schrijfIn(&mc);
    jane.schrijfIn(&ip);

    // professor
    Professor toon("Toon", "Calders");
    toon.geeft(&ip);
    Professor els("Els", "Laenens");
    els.geeft(&gas);
    vector<Professor*> universiteit2;
    universiteit2.push_back(&toon);
    universiteit2.push_back(&els);

    // assistenten
    Assistent tom("Tom", "Hofkens");
    tom.setBegeleider(&toon);
    Assistent daphne("Daphne", "Lenders");
    daphne.setBegeleider(&toon);
    tom.geeft(&ip);
    tom.geeft(&gas);
    daphne.geeft(&ip);
    vector<Assistent*> universiteit3;
    universiteit3.push_back(&tom);
    universiteit3.push_back(&daphne);

    // print alles
    cout << "Iedereen verbonden aan de universiteit:" << endl;
    for(Student* member : universiteit){
        cout << '\t' << member->toString() << endl;
    }
    for(Professor* member : universiteit2){
        cout << '\t' << member->toString() << endl;
    }
    for(Assistent* member : universiteit3){
        cout << '\t' << member->toString() << endl;
    }

    cout << "Alle cursussen:" << endl;
    for(Cursus* course : cursussen){
        course->print();
    }

    jane.schrijfUit(&ip);

    // print cursussen
    cout << "Alle cursussen:" << endl;
    for(Cursus* cursus : cursussen){
        cursus->print();
    }
    return 0;
}
int Oef5() {
    // student
    Student john("John", "Doe", "12345");
    Student jane("Jane", "Doe", "54321");
    vector<Student*> universiteit;
    universiteit.push_back(&john);
    universiteit.push_back(&jane);

    // course
    Cursus ip("IP", "Inleiding programmeren", 9);
    Cursus gas("GAS", "Gegevensabstractie en -structuren", 6);
    Cursus mc("MC", "Monster Cursus", 56);

    vector<Cursus*> cursussen;
    cursussen.push_back(&ip);
    cursussen.push_back(&gas);
    cursussen.push_back(&mc);

    // inschrijven
    john.schrijfIn(&ip);
    john.schrijfIn(&gas);
    john.schrijfIn(&mc);
    jane.schrijfIn(&ip);

    // professor
    Professor toon("Toon", "Calders");
    toon.geeft(&ip);
    Professor els("Els", "Laenens");
    els.geeft(&gas);
    vector<Professor*> universiteit2;
    universiteit2.push_back(&toon);
    universiteit2.push_back(&els);

    // assistenten
    Assistent tom("Tom", "Hofkens");
    tom.setBegeleider(&toon);
    Assistent daphne("Daphne", "Lenders");
    daphne.setBegeleider(&toon);
    tom.geeft(&ip);
    tom.geeft(&gas);
    daphne.geeft(&ip);
    toon.voegAssistentToe(&daphne);
    toon.voegAssistentToe(&tom);
    els.voegAssistentToe(&tom);
    vector<Assistent*> universiteit3;
    universiteit3.push_back(&tom);
    universiteit3.push_back(&daphne);

    // print alles
    for(Student* member : universiteit){
        member->showHomepage();
    }
    for(Professor* member : universiteit2){
        member->showHomepage();
    }
    for(Assistent* member : universiteit3){
        member->showHomepage();
    }
    return 0;
}
int main() {
    cout << endl << "==Oefening 1==" << endl;
    Oef1();
    cout << endl << "==Oefening 2==Oefening 3==" << endl;
    Oef2();
    cout << endl << "==Oefening 4==" << endl;
    Oef4();
    cout << endl << "==Oefening 5==" << endl;
    Oef5();
    return 0;
}

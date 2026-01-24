#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <vector>
#include "Actor.h"
#include "Director.h"
using namespace std;

class Movie {
private:
    int year;
    Director* director;
    string title;
    string genre;
    vector<Actor*> actors;
    double rating;

public:
    // Constructor
    Movie(int year, Director* director, const string& title, const string& genre,
          const vector<Actor*>& actors, double rating);

    int getYear() const;

    void setYear(int year);

    Director *getDirector() const;

    void setDirector(Director *director);

    const string &getTitle() const;

    void setTitle(const string &title);

    const string &getGenre() const;

    void setGenre(const string &genre);

    const vector<Actor *> &getActors() const;

    void setActors(const vector<Actor *> &actors);

    double getRating() const;

    void setRating(double rating);

    // Additional Methods
    string toString() const;
    void starring(Actor* actor);
};

#endif

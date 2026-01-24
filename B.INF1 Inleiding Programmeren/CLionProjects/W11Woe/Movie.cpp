#include "Movie.h"
#include <sstream>
#include "Actor.h"
#include "Director.h"
using namespace std;

// Constructor
Movie::Movie(int year, Director* director, const string& title, const string& genre,
             const vector<Actor*>& actors, double rating)
    : year(year), director(director), title(title), genre(genre), actors(actors), rating(rating) {}


string Movie::toString() const {
    ostringstream output;
    output << title << " (" << year << ")";
    return output.str();
}

void Movie::starring(Actor* actor) {
    actors.push_back(actor);
}

int Movie::getYear() const {
    return year;
}

void Movie::setYear(int year) {
    Movie::year = year;
}

Director *Movie::getDirector() const {
    return director;
}

void Movie::setDirector(Director *director) {
    Movie::director = director;
}

const string &Movie::getTitle() const {
    return title;
}

void Movie::setTitle(const string &title) {
    Movie::title = title;
}

const string &Movie::getGenre() const {
    return genre;
}

void Movie::setGenre(const string &genre) {
    Movie::genre = genre;
}

const vector<Actor *> &Movie::getActors() const {
    return actors;
}

void Movie::setActors(const vector<Actor *> &actors) {
    Movie::actors = actors;
}

double Movie::getRating() const {
    return rating;
}

void Movie::setRating(double rating) {
    Movie::rating = rating;
}


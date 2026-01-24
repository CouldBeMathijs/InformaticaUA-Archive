#include "IMDB.h"
#include "Movie.h"
#include <cmath>
IMDB::IMDB() {}

IMDB::IMDB(const vector<Movie*> &movies) : movies(movies) {}

void IMDB::addMovie(Movie* Movie) {
    movies.push_back(Movie);
}

string IMDB::toString() {
    string out;
    for (auto movie:movies){
        out += movie->toString() + "\n";
    }
    return out;
}

Movie* IMDB::getBest() {
    Movie* highestRated = movies[0];
    for (const auto& movie : movies) {
        if (movie->getRating() > highestRated->getRating()) {
            highestRated = movie;
        }
    }
    return highestRated;
}

vector<Movie*> IMDB::getSimilar(Movie *Similar) {
    vector<Movie*> vecsim;
    for (const auto& movie : movies) {
        if (movie->getGenre() == Similar->getGenre() ||
            abs(movie->getRating() - Similar->getRating()) < 1.0) {
            if (movie->getTitle() != Similar->getTitle()) { // Avoid adding the same movie
                vecsim.push_back(movie);
            }
        }
    }
    return vecsim;
}


#ifndef W11WOE_IMDB_H
#define W11WOE_IMDB_H

#include <vector>
#include "Movie.h"

class IMDB {
private:
    vector<Movie*> movies;
public:
    IMDB();

    explicit IMDB(const vector<Movie*> &movies);

    void addMovie(Movie * Movie);
    string toString();
    Movie* getBest();
    vector<Movie*> getSimilar(Movie*Movie);
};


#endif //W11WOE_IMDB_H

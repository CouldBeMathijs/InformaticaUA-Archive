#include <string>
#include <iostream>
using namespace std;

class MovieProvider{
private:
    string* movies;
    int maxMovies;
    int totMovies;
public:
    MovieProvider(int maxMovies) : maxMovies(maxMovies), totMovies(0) {
        string* movies = new string[maxMovies];
    }
    virtual void setRating(string movie, double rating) = 0;

    ~MovieProvider() {
        delete movies;
    }

    string getMovie(int index){
        return movies[index];
    }

    int getMaxMovies() const {
        return maxMovies;
    }
    int getTotMovies() const {
        return totMovies;
    }
    void addMovie(string movie){
        movies[++totMovies] = movie;
    }

    void print(){
        for(int i = 0; i < maxMovies; i++) {
            cout << movies[i] << endl;
        }
    }

protected:
    int getIndex(const string& movie){
        for(int i = 0; i < maxMovies; i++){
            if(movies[i] == movie) return i;
        }
        return -1;
    }
};

class Netflix: public MovieProvider{
private:
    double* ratings; // ratings[0] = 0.9 is de rating van movies[0]
public:
    Netflix(int maxMovies) : MovieProvider(maxMovies) {
        double* ratings = new double;
    }

    ~Netflix() {
        delete ratings;
    }

    void setRating(string movie, double rating){
        int index = getIndex(movie);
        if(index >= 0){
            ratings[index] = rating;
        }
    }

    void print(){
        for(int i = 0; i < getMaxMovies(); i++) {
            cout << getMovie(i) << " (rating: " << ratings[i] << ")" << endl;
        }
    }

};

#include <iostream>

using namespace std;
#include "Movie.h"
#include "IMDB.h"
#include "Actor.h"
#include "Director.h"
int main() {
    //Actors
    Actor daniel_radcliffe{"Daniel", "Radcliffe"};
    Actor* p_dr = &daniel_radcliffe;
    Actor emma_watson{"Emma", "Watson"};
    Actor* p_ew = &emma_watson;
    Actor rupert_grint{"Rupert", "Grint"};
    Actor graham_chapman{"Graham",  "Chapman"};
    Actor *p_gc = &graham_chapman;
    Actor john_cleese = {"John", "Cleese"};
    Actor* p_jc = &john_cleese;
    Actor robert_pattinson{"Robert", "Pattinson"};
    Actor* p_rp = &robert_pattinson;
    Actor kristen_stewart{"Kristen", "Stewart"};
    Actor* p_ks = &kristen_stewart;
    Actor choi = {"Choi",  "Woo-Shik"};
    Actor* p_choi = &choi;
    Actor park = {"Park", "So-dam"};
    Actor* p_park = &park;
    Actor elizabeth{"Elizabeth",  "Debicki"};
    Actor* p_elizabeth = &elizabeth;


    //Movie 1
    int movie1_year = 2001;
    Director movie1_director{"Chris", "Columbus"};
    Director* p_movie1_director = &movie1_director;
    string movie1_title = "Harry Potter and the Philosopher's Stone";
    string movie1_genre = "Fantasy";
    vector<Actor*> movie1_actors = {p_dr, p_ew, p_gc};
    double movie1_rating = 7.4;
    Movie movie1{movie1_year, p_movie1_director, movie1_title, movie1_genre, movie1_actors, movie1_rating};
    Movie* p_movie1 = &movie1;

    //Movie 2
    int movie2_year = 1975;
    Director movie2_director{"Terry", "Jones"};
    Director* p_movie2_director = &movie2_director;
    string movie2_title = "Monty Python and the Holy Grail";
    string movie2_genre = "Comedy";
    vector<Actor*> movie2_actors = {p_gc, p_jc};
    double movie2_rating = 8.1;
    Movie movie2{movie2_year, p_movie2_director, movie2_title, movie2_genre, movie2_actors, movie2_rating};
    Movie* p_movie2 = &movie2;

    //Movie 3
    int movie3_year = 2007;
    Director movie3_director{"Catherine", "Hardwicke"};
    Director* p_movie3_director = &movie3_director;
    string movie3_title = "Twilight";
    string movie3_genre = "Fantasy";
    vector<Actor*> movie3_actors = {p_rp, p_ks};
    double movie3_rating = 5.2;
    Movie movie3{movie3_year, p_movie3_director, movie3_title, movie3_genre, movie3_actors, movie3_rating};
    Movie* p_movie3 = &movie3;

    //Movie 4
    int movie4_year = 2019;
    Director movie4_director{"Bong", "Joon-ho"};
    Director* p_movie4_director = &movie4_director;
    string movie4_title = "Parasite";
    string movie4_genre = "Thriller";
    vector<Actor*> movie4_actors = {p_choi, p_park};
    double movie4_rating = 8.6;
    Movie movie4{movie4_year, p_movie4_director, movie4_title, movie4_genre, movie4_actors, movie4_rating};
    Movie* p_movie4 = &movie4;

    //Movie 5
    int movie5_year = 2020;
    Director movie5_director{"Christopher", "Nolan"};
    Director* p_movie5_director = &movie5_director;
    string movie5_title = "Tenet";
    string movie5_genre = "Action";
    vector<Actor*> movie5_actors = {p_rp, p_elizabeth};
    double movie5_rating = 7.8;
    Movie movie5{movie5_year, p_movie5_director, movie5_title, movie5_genre, movie5_actors, movie5_rating};
    Movie* p_movie5 = &movie4;

    vector<Movie*> movies{p_movie1, p_movie2, p_movie3};
    IMDB imdb{movies};

    cout<<imdb.toString()<<endl;

    vector<Movie*> similarMovies = imdb.getSimilar(p_movie4);
    for (Movie* similarMovie: similarMovies) {
        cout<<similarMovie->toString()<<endl;
    }
}

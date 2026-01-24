//
// Created by Tim Apers on 10/01/2025.
//

#include "Game.h"
#include <cmath>
#include <iostream>

Game::~Game() {
    for (auto tile : tiles) {
        delete tile.second;
    }
}

void Game::render() {
    int size = sqrt(tiles.size());
    for(int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << tiles[{i, j}]->visualize();
        }
        cout << endl;
    }
}

void Game::move(pair<int, int> direction) {
    direction = quantizeDirection(direction);

    // Keep moving until no more tiles can be moved
    bool moved = true;
    while (moved) {
        moved = false;

        // Loop through in normal order if moving left or up
        if (direction.first < 0 or direction.second < 0) {
            for (auto tile : tiles) {
                moved = moveTile(tile.first, direction) ? true : moved;
            }
        }

        // Loop through in reverse order if moving right or down
        else {
            vector<pair<pair<int, int>, Tile*>> reversed(tiles.rbegin(), tiles.rend());
            for (auto tile : reversed) {
                moved = moveTile(tile.first, direction) ? true : moved;
            }
        }
    }
}

bool Game::moveTile(pair<int, int> pos, pair<int, int> direction) {
    return false;
}

pair<int, int> Game::quantizeDirection(pair<int, int> direction) {
    return direction;
}
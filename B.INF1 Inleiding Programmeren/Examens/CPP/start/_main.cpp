//
// Created by Tim Apers on 06/01/2025.
//

#include <iostream>
#include "WallTile.h"
#include "EmptyTile.h"
#include "ValueTile.h"
#include "Game.h"

using namespace std;

int main() {

    // TODO: Vraag 1
    /*
    WallTile* wall = new WallTile();
    EmptyTile* empty = new EmptyTile();

    cout << wall->visualize() << endl;
    cout << empty->visualize() << endl;

    for (int i = 2; i <= 128; i *= 2) {
        ValueTile* value = new ValueTile(i);
        cout << value->visualize() << endl;
        delete value;
    }
    */

    // TODO: Vraag 2
    /*
    Game* game = new Game(4);
    game->render();
    */

    // TODO: Vraag 3
    /*
    Game* game = new Game(4);
    game->setValue(1, 1, 4);
    game->setValue(1, 2, 2);
    game->setValue(1, 3, 2);
    game->setValue(1, 4, 4);
    game->setValue(2, 1, 16);
    game->setValue(2, 2, 16);
    game->setValue(3, 1, 64);
    game->setValue(3, 2, 8);
    game->setValue(4, 1, 8);
    game->setValue(4, 3, 8);
    game->render();
    */

    // TODO: Vraag 4
    /*
    ValueTile* value1 = new ValueTile(2);
    ValueTile* value2 = new ValueTile(2);
    ValueTile* value3 = new ValueTile(4);
    WallTile* wall = new WallTile();
    EmptyTile* empty = new EmptyTile();

    cout << value1->replaceBy(value2) << endl;
    cout << value1->replaceBy(value3) << endl;
    cout << value1->replaceBy(wall) << endl;
    cout << value1->replaceBy(empty) << endl;
    cout << wall->replaceBy(value1) << endl;
    cout << wall->replaceBy(wall) << endl;
    cout << wall->replaceBy(empty) << endl;
    cout << empty->replaceBy(value1) << endl;
    cout << empty->replaceBy(wall) << endl;
    cout << empty->replaceBy(empty) << endl;
    */

    // TODO: Vraag 5
    /*
    Game* game = new Game(4);
    pair<int, int> result1 = game->quantizeDirection({3, 1});
    pair<int, int> result2 = game->quantizeDirection({1, 5});
    pair<int, int> result3 = game->quantizeDirection({-4, 2});
    pair<int, int> result4 = game->quantizeDirection({0, -7});

    cout << result1.first << ", " << result1.second << endl;
    cout << result2.first << ", " << result2.second << endl;
    cout << result3.first << ", " << result3.second << endl;
    cout << result4.first << ", " << result4.second << endl;
    */

    // TODO: Vraag 6
    /*
    Game* game = new Game(4);
    game->setValue(1, 1, 4);
    game->setValue(1, 2, 2);
    game->setValue(1, 3, 2);
    game->setValue(1, 4, 4);
    game->setValue(2, 1, 16);
    game->setValue(2, 2, 16);
    game->setValue(3, 1, 64);
    game->setValue(3, 2, 8);
    game->setValue(4, 1, 8);
    game->setValue(4, 3, 8);
    cout << game->moveTile({2, 2}, {0, -1}) << endl;
    cout << game->moveTile({4, 3}, {1, 0}) << endl;
    cout << game->moveTile({4, 3}, {0, -1}) << endl;
    cout << game->moveTile({4, 2}, {0, -1}) << endl;
    cout << game->moveTile({5, 4}, {-1, 0}) << endl;
    cout << game->moveTile({3, 2}, {0, -1}) << endl;
    cout << game->moveTile({1, 4}, {1, 0}) << endl;
    game->render();
    */

    // TODO: Vraag 7
    /*
    Game* game = new Game(4);
    game->setValue(1, 1, 4);
    game->setValue(1, 2, 2);
    game->setValue(1, 3, 2);
    game->setValue(1, 4, 4);
    game->setValue(2, 1, 16);
    game->setValue(2, 2, 16);
    game->setValue(3, 1, 64);
    game->setValue(3, 2, 8);
    game->setValue(4, 1, 8);
    game->setValue(4, 3, 8);

    game->render();
    game->move({0, -1}); // Up
    game->render();
    game->move({1, 0}); // Right
    game->render();
    game->move({0, 1}); // Down
    game->render();
    game->move({-1, 0}); // Left
    game->render();
    game->move({1, 0}); // Right
    game->render();
    game->move({0, 1}); // Down
    game->move({0, 1}); // Down
    game->render();
    game->move({-1, 0}); // Left
    game->move({1, 0}); // Right
    game->move({0, -1}); // Up
    game->render();
    */

    return 0;
}
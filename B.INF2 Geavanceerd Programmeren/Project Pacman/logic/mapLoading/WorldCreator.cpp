#include "WorldCreator.h"

#include "../ScoreKeeper.h"
#include "../patterns/IEntityFactory.h"
#include "IWorldConfigParser.h"

World WorldCreator::createWorld(const std::string& filename, const IWorldConfigParser& parser,
                                const std::shared_ptr<IEntityFactory>& factory) {

    const std::vector<std::vector<char>> gridData = parser.parse(filename);

    World out;
    auto& score = ScoreKeeper::getInstance();

    const size_t num_rows_y = gridData.size();    // Y-dimension (height)
    const size_t num_cols_x = gridData[0].size(); // X-dimension (width)

    // 4. Loop in the standard [Y][X] order (Row by Row, then Column by Column)
    for (size_t y = 0; y < num_rows_y; y++) {     // Loop over rows (Y coordinate)
        for (size_t x = 0; x < num_cols_x; x++) { // Loop over columns (X coordinate)

            // Step A: Calculate the top-left corner of the current grid cell (x, y)
            // This represents the start of the cell.
            Position pos_start =
                Position(static_cast<double>(x), static_cast<double>(y))
                    .rescale({0, 0}, {static_cast<double>(num_cols_x), static_cast<double>(num_rows_y)},
                             {-1, -LogicConstants::REVERSE_TARGET_ASPECT_RATIO},
                             {1, LogicConstants::REVERSE_TARGET_ASPECT_RATIO});

            // Step B: Calculate the bottom-right corner of the current grid cell (x+1, y+1)
            // This represents the end of the cell.
            Position pos_end = Position(static_cast<double>(x + 1), static_cast<double>(y + 1))
                                   .rescale({0, 0}, {static_cast<double>(num_cols_x), static_cast<double>(num_rows_y)},
                                            {-1, -LogicConstants::REVERSE_TARGET_ASPECT_RATIO},
                                            {1, LogicConstants::REVERSE_TARGET_ASPECT_RATIO});

            // Hitbox (hb) spans the entire grid cell (100% size) for ALL entities,
            // ensuring they are all the same size relative to the grid.
            Rectangle hb = {pos_start, pos_end};
            Rectangle hb_coin = hb.scaledBy(0.2);

            // Access the grid using [row][column] which is [y][x]
            switch (gridData[y][x]) {
            case '#': { // Wall
                auto w = factory->createWall(hb);
                out.addNonMovingEntity(std::move(w));
            } break;

            case '*': { // Coin
                auto c = factory->createCoin(hb_coin);
                score.addCollectable();
                out.addNonMovingEntity(std::move(c));
            } break;

            case 'F': { // Fruit
                auto f = factory->createFruit(hb);
                score.addCollectable();
                out.addNonMovingEntity(std::move(f));
            } break;

            case 'P': { // Pacman
                auto p = factory->createPacman(hb);
                out.setPacman(std::static_pointer_cast<Pacman>(std::move(p)));
            } break;

            case '1': { // Blue Ghost
                auto g = factory->createBlueGhost(hb);
                out.addGhost(std::static_pointer_cast<IGhost>(std::move(g)));
            } break;

            case '2': { // Pink Ghost
                auto g = factory->createPinkGhost(hb);
                out.addGhost(std::static_pointer_cast<IGhost>(std::move(g)));
            } break;

            case '3': { // Orange Ghost
                auto g = factory->createOrangeGhost(hb);
                out.addGhost(std::static_pointer_cast<IGhost>(std::move(g)));
            } break;

            case '4': { // Red Ghost
                auto g = factory->createRedGhost(hb);
                out.addGhost(std::static_pointer_cast<IGhost>(std::move(g)));
            } break;

            case '_': {
                auto w = factory->createSpawnWall(hb);
                out.addNonMovingEntity(std::move(w));
            } break;

            default:
                // Do nothing for empty space ' '
                break;
            }
        }
    }
    return out;
}
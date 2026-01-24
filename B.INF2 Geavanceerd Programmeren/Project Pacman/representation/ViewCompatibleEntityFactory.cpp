#include "ViewCompatibleEntityFactory.h"

#include "entityType/Collectable.h"
#include "entityType/Ghost.h"
#include "entityType/Pacman.h"
#include "entityType/SpawnWall.h"
#include "entityType/Wall.h"
#include "view/CoinView.h"
#include "view/FruitView.h"
#include "view/GhostView.h"
#include "view/PacmanView.h"
#include "view/ScoreView.h"
#include "view/SpawnWallView.h"
#include "view/WallView.h"

std::shared_ptr<IEntityModel> ViewCompatibleEntityFactory::createPacman(const Rectangle& p) {
    auto pacman = createEntityWithView<Pacman, PacmanView>(p);
    const auto scoreView = std::make_shared<ScoreView>();
    pacman->addObserver(scoreView);
    m_viewStorage.push_back(scoreView);
    return pacman;
}

std::shared_ptr<IEntityModel> ViewCompatibleEntityFactory::createWall(const Rectangle& p) {
    return createEntityWithView<Wall, WallView>(p);
}

ViewCompatibleEntityFactory::ViewCompatibleEntityFactory(std::vector<std::shared_ptr<IObserver>>& viewStorage)
    : m_viewStorage(viewStorage) {}

std::shared_ptr<IEntityModel> ViewCompatibleEntityFactory::createSpawnWall(const Rectangle& hb) {
    return createEntityWithView<SpawnWall, SpawnWallView>(hb);
}

std::shared_ptr<IEntityModel> ViewCompatibleEntityFactory::createCoin(const Rectangle& p) {
    return createEntityWithView<Coin, CoinView>(p);
}

std::shared_ptr<IEntityModel> ViewCompatibleEntityFactory::createFruit(const Rectangle& p) {
    return createEntityWithView<Fruit, FruitView>(p);
}

std::shared_ptr<IEntityModel> ViewCompatibleEntityFactory::createOrangeGhost(const Rectangle& p) {
    return createEntityWithView<OrangeGhost, OrangeGhostView>(p);
}

std::shared_ptr<IEntityModel> ViewCompatibleEntityFactory::createRedGhost(const Rectangle& p) {
    return createEntityWithView<RedGhost, RedGhostView>(p);
}

std::shared_ptr<IEntityModel> ViewCompatibleEntityFactory::createBlueGhost(const Rectangle& p) {
    return createEntityWithView<BlueGhost, BlueGhostView>(p);
}

std::shared_ptr<IEntityModel> ViewCompatibleEntityFactory::createPinkGhost(const Rectangle& p) {
    return createEntityWithView<PinkGhost, PinkGhostView>(p);
}
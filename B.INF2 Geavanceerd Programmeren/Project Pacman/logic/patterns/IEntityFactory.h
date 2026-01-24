#ifndef PACMAN_ABSTRACT_ENTITY_FACTORY_H
#define PACMAN_ABSTRACT_ENTITY_FACTORY_H
#include "../entityType/IEntityModel.h"

#include <memory>

/**
 * @brief Pure virtual EntityModel factory
 */
class IEntityFactory {
protected:
    IEntityFactory() = default;

public:
    virtual ~IEntityFactory() = default;

    virtual std::shared_ptr<IEntityModel> createBlueGhost(const Rectangle& pos) = 0;
    virtual std::shared_ptr<IEntityModel> createCoin(const Rectangle& pos) = 0;
    virtual std::shared_ptr<IEntityModel> createFruit(const Rectangle& pos) = 0;
    virtual std::shared_ptr<IEntityModel> createOrangeGhost(const Rectangle& pos) = 0;
    virtual std::shared_ptr<IEntityModel> createPacman(const Rectangle& pos) = 0;
    virtual std::shared_ptr<IEntityModel> createPinkGhost(const Rectangle& pos) = 0;
    virtual std::shared_ptr<IEntityModel> createRedGhost(const Rectangle& pos) = 0;
    virtual std::shared_ptr<IEntityModel> createSpawnWall(const Rectangle& hb) = 0;
    virtual std::shared_ptr<IEntityModel> createWall(const Rectangle& pos) = 0;
};

#endif

#ifndef PACMAN_VIEW_COMPATIBLE_ENTITY_FACTORY_H
#define PACMAN_VIEW_COMPATIBLE_ENTITY_FACTORY_H

#include "../logic/patterns/IEntityFactory.h"

#include <memory>

/**
 * @brief Concrete Entity Factory which adds entity views as observers on object creation
 */
class ViewCompatibleEntityFactory final : public IEntityFactory {
public:
    explicit ViewCompatibleEntityFactory(std::vector<std::shared_ptr<IObserver>>& viewStorage);
    std::shared_ptr<IEntityModel> createBlueGhost(const Rectangle& p) override;
    std::shared_ptr<IEntityModel> createCoin(const Rectangle& p) override;
    std::shared_ptr<IEntityModel> createFruit(const Rectangle& p) override;
    std::shared_ptr<IEntityModel> createOrangeGhost(const Rectangle& p) override;
    std::shared_ptr<IEntityModel> createPacman(const Rectangle& p) override;
    std::shared_ptr<IEntityModel> createPinkGhost(const Rectangle& p) override;
    std::shared_ptr<IEntityModel> createRedGhost(const Rectangle& p) override;
    std::shared_ptr<IEntityModel> createSpawnWall(const Rectangle& hb) override;
    std::shared_ptr<IEntityModel> createWall(const Rectangle& p) override;

private:
    std::vector<std::shared_ptr<IObserver>>& m_viewStorage;

    /**
     * @brief Template helper function to create an Entity Model and attach its View Observer.
     * @tparam Model The specific EntityModel subclass (e.g., Pacman, RedGhost).
     * @tparam View The specific View class that observes the Model.
     * @param p The position for the new entity.
     * @return A shared_ptr to the newly created Model, cast up to the base EntityModel.
     */
    template <typename Model, typename View>
    std::shared_ptr<IEntityModel> createEntityWithView(Rectangle p) {
        auto model = std::make_shared<Model>(p);
        auto view = std::make_shared<View>(model);

        model->addObserver(view);
        m_viewStorage.push_back(view);

        return model;
    }
};

#endif

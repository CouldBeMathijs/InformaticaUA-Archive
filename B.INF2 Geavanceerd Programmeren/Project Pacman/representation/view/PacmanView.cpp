#include "PacmanView.h"

#include "entityType/Pacman.h"

PacmanView::PacmanView(std::shared_ptr<IEntityModel> model)
    : IDirectionalEntityView(Assets::getSpriteInfo(Assets::SpriteDefinition::PacmanBase), std::move(model), 3) {}

void PacmanView::update() {

    if (const double deathTimer = std::static_pointer_cast<Pacman>(getCoupledEntity())->getDeathTimer();
        deathTimer > 0) {
        if (!m_dying) {
            m_dying = !m_dying;
            m_amountOfTextures = 11;
            m_currentSprite = Assets::getSpriteInfo(Assets::SpriteDefinition::PacmanDeathBase);
            m_currentTextureOffset = 0;
        }
        IEntityView::update();
    } else {
        if (m_dying) {
            m_dying = !m_dying;
            m_amountOfTextures = 3;
            m_currentSprite = Assets::getSpriteInfo(Assets::SpriteDefinition::PacmanBase);
            m_currentTextureOffset = 0;
        }
        IDirectionalEntityView::update();
    }
}
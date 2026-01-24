#include "GhostView.h"

#include "entityType/Ghost.h"

IGhostView::IGhostView(const Assets::TextureLocation t, std::shared_ptr<IEntityModel> e)
    : IDirectionalEntityView(t, std::move(e), 2), m_baseSprite(t) {}

void IGhostView::update() {
    switch (std::static_pointer_cast<IGhost>(getCoupledEntity())->getMode()) {
    case GhostMode::WAITING:
    case GhostMode::DEAD:
    case GhostMode::CHASING:
        if (m_currentSprite != m_baseSprite) {
            m_currentSprite = m_baseSprite;
            m_topBase = m_currentSprite.top;
        }
        IDirectionalEntityView::update();
        break;
    case GhostMode::PANICKING:
        auto panic_sprite = Assets::getSpriteInfo(Assets::SpriteDefinition::GhostPanicBase);
        if (std::static_pointer_cast<IGhost>(getCoupledEntity())->getTimeInCurrentMode() < 1) {
            panic_sprite = Assets::getSpriteInfo(Assets::SpriteDefinition::GhostPanicAlternate);
        }
        if (m_currentSprite != panic_sprite) {
            m_currentSprite = panic_sprite;
            m_topBase = m_currentSprite.top;
        }
        IDirectionalEntityView::update();
        break;
    }
}

BlueGhostView::BlueGhostView(std::shared_ptr<IEntityModel> e)
    : IGhostView(Assets::getSpriteInfo(Assets::SpriteDefinition::GhostBlueBase), std::move(e)) {}

PinkGhostView::PinkGhostView(std::shared_ptr<IEntityModel> e)
    : IGhostView(Assets::getSpriteInfo(Assets::SpriteDefinition::GhostPinkBase), std::move(e)) {}

OrangeGhostView::OrangeGhostView(std::shared_ptr<IEntityModel> e)
    : IGhostView(Assets::getSpriteInfo(Assets::SpriteDefinition::GhostOrangeBase), std::move(e)) {}

RedGhostView::RedGhostView(std::shared_ptr<IEntityModel> e)
    : IGhostView(Assets::getSpriteInfo(Assets::SpriteDefinition::GhostRedBase), std::move(e)) {}

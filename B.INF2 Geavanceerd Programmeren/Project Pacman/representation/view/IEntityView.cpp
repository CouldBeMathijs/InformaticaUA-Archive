#include "IEntityView.h"

#include "IDirectionalEntityView.h"

#include <utility>

#include "../Camera.h"
#include "../SfmlConstants.h"
#include "Position.h"
#include "Stopwatch.h"

#include <memory>

IEntityView::IEntityView(Assets::TextureLocation m, std::shared_ptr<IEntityModel> n)
    : m_coupledEntity(std::move(n)), m_currentSprite(m) {}

void IEntityView::update() {
    auto& camera = Camera::getInstance();
    auto& window = camera.getWindow();
    animate();
    const Rectangle p = m_coupledEntity->getHitBox().rescale({-1, -LogicConstants::REVERSE_TARGET_ASPECT_RATIO},
                                                             {1, LogicConstants::REVERSE_TARGET_ASPECT_RATIO}, {0, 0},
                                                             {SfmlConstants::VIEW_WIDTH, SfmlConstants::VIEW_HEIGHT});

    sf::Sprite sprite(Assets::getSpritemapTexture(), m_currentSprite);

    const auto targetWidth = static_cast<float>(std::abs(p.topLeft.x - p.bottomRight.x));
    const auto targetHeight = static_cast<float>(std::abs(p.topLeft.y - p.bottomRight.y));

    const auto currentWidth = static_cast<float>(m_currentSprite.width);
    const auto currentHeight = static_cast<float>(m_currentSprite.height);

    const float scaleX = targetWidth / currentWidth;
    const float scaleY = targetHeight / currentHeight;

    sprite.setScale(scaleX, scaleY);

    sprite.setPosition(static_cast<float>(p.topLeft.x), static_cast<float>(p.topLeft.y));

    window.draw(sprite);
}

void IEntityView::animate() {
    if (m_amountOfTextures <= 1) {
        return;
    }
    m_timeAccumulator += Stopwatch::getInstance().getDeltaTime();

    if (m_timeAccumulator < m_timePerFrame) {
        return;
    }

    m_timeAccumulator -= m_timePerFrame;

    if (m_animationCycleMovingUp && m_currentTextureOffset == m_amountOfTextures - 1) {
        m_animationCycleMovingUp = false;
    } else if (!m_animationCycleMovingUp && m_currentTextureOffset == 0) {
        m_animationCycleMovingUp = true;
    }

    if (m_animationCycleMovingUp) {
        m_currentTextureOffset += 1;
        m_currentSprite.top += 50;
    } else {
        m_currentTextureOffset -= 1;
        m_currentSprite.top -= 50;
    }
}

unsigned int IEntityView::getCurrentTextureOffset() const { return m_currentTextureOffset; }

std::shared_ptr<IEntityModel> IEntityView::getCoupledEntity() { return m_coupledEntity; }
